#!/usr/bin/python                                                               
""" calc_ab_stats.py
Calculates and plots comparison metrics as a function of credibility for
results of an AB test

Args:
    NA = number of trials in variant A
    MA = number of successes in variant A
    MB = number of trials in variant B
    MB = number of successes in variant B

    DELTA = pA - pB (= 0.1)
    LIFT = pA/pB - 1 (= 0.2)
    ODDSFACTOR = pA/(1-pA)/(pB/(1-pB)) (= 1.2)
    
Returns:
    jpeg and csv of Bayesian credibility vs. comparison metric
    parametric and non-parametric credibilites for above threshold differences 
"""

import numpy as np
import os
import pandas as pd
import sys

from math import sqrt, erf, exp
from scipy.stats import beta, norm
from subprocess import call

sys.path.insert(0, os.path.abspath('..'))

from lib import utilities as u
from lib import ab_stats_functions as fn
from lib import ab_stats_metrics as abm

from metrics import metricsClassR as mc

def Bayesian_prob_from_quantile(qq, n, m):
    """ For m successes from n trials, calculates the probability 
    at which the cumulative of the Bayesian Post. dist. has a 
    given quantile value qq"""
    return beta.ppf(qq, m + 1, n - m + 1)
    

def main():
    supp = 0 # do NOT produce supplementary figures for document or examination
    if len(sys.argv) == 2:
        configF = sys.argv[1]
        # supp = 0 # no supplementary figures
    else:
        print "Usage: "
        print "calc_ab_stats.py config_file"
        print "See 'ABConfig_rst' for an example"
        print "No file or arg list found"
        print "Using 'ABConfig_rst' as default config file"
        configF = "ABConfig_rst"

    try:
        f = open(configF, "r")
    except IOError:
        print "config file not found, exiting"
        sys.exit(1)

    supp = 0
    lnum = 0
    dataF = ""
    minAccD = {} # minimum acceptable return
    mtrCL = mc.metricC.metricsL
    mcnameL = map(lambda i: i.name, mtrCL) #metrics in class
    print "\nMetrics defined in Class =", mcnameL
    for l in f:
        lL = l.strip().split(' ')
        if lnum == 0:
            if lL[0].strip() == "supp":
                supp = 1
        elif 1 <= lnum <= 3:
            dataF += '_'+lL[0]
        else:
            mname = lL[0].strip()
            if mname in mcnameL:
                try:
                    minexp = float(lL[1].strip())
                except ValueError as XErr:
                    print XErr, "Min. Acceptable Value for", mname, "not float"
                    print "for ", lL[0], "exiting"
                    sys.exit(1)
                minAccD[mname] = minexp
            else:
                print "\nConfig Error: metric", mname, "in", configF
                print mname, "not in metrics Class list"
                print "Continuing with rest"
                # sys.exit(1)
        lnum += 1
    f.close()

    # check valid range for minimum expected value of metrics
    for mtr in mtrCL:
        minm = mtr.lower_bound
        maxm = mtr.upper_bound
        mname = mtr.name
        # print mname, minm, maxm
        try:
            if minm <= minAccD[mname] <= maxm:
                # print "Min. Acceptable value for", mname, "in range"
                pass
            else:
                print "Min. Acceptable value for", mname, "out of range, exiting"
                sys.exit(1)
        except KeyError:
            print mname, "not in config file"
            pass

    dataFpath = "../data/"+dataF+".csv"
    f = open(dataFpath, 'r')
    dat1L = f.readline().strip().split(',')
    if len(dat1L) != 4:
        print "Unexpected number of arguments, 4 expected, exiting"
        sys.exit(1)
    try:
        NA = int(dat1L[0].strip())
        MA = int(dat1L[1].strip())
        NB = int(dat1L[2].strip())
        MB = int(dat1L[3].strip())
    except ValueError as XErr:
        print XErr, "Invalid argument, exiting"
        sys.exit(1)

    # Check validity of input values
    if (NA <= 0 or MA < 0 or NB <= 0 or MB < 0):
        print "Negative count encountered, exiting"
        sys.exit(1)

    if (MA > NA or MB > NB):
        print "Inequalities in counts not satisfied, exiting"
        sys.exit(1)

    print "\nINPUT: A = Variant, B = Control"
    print "NA, MA, NB, MB = ", NA, MA, NB, MB, '\n'
    for mtr in minAccD:
        print "Min. Exp. return for", mtr, "= ", minAccD[mtr]


    # calculate the Frequentist descriptive stat.s
    # for both "A"=test and "B"=control
    # and the Frequentist confidence that A is better than B,
    # i.e. p_A > p_B
    MUA = fn.mu_freq(NA, MA)
    VARMUA = fn.var_mu_freq(NA, MA)
    MUB = fn.mu_freq(NB, MB)
    VARMUB = fn.var_mu_freq(NB, MB)
    ZPROP = (MUA - MUB)/sqrt(VARMUA + VARMUB)
    FREQ_CONF = norm.cdf(ZPROP)

    print "\n\nFrequentist Confidence Level"
    print "Conf[A better than B] =", round(FREQ_CONF*100, 1), "%"

    ## Bayesian Posterior distribution and properties
    # Construct lists or arrays of p_A and p_B which span 99.9% of the volume
    # of the Bayesian Posterior distribution
    # for plotting or for numerical integration
    # These arrays and lists are CONSTANT
    # Though the values do depend on NA, MA etc,
    # they are fixed for the duration of the execution
    # and have 'forgotten' about NA, MA, so might as well pass them
    # instead of recalculating them
    
    QQA = u.make_quantile_array(0.998, 999)

    PROBAAr = Bayesian_prob_from_quantile(QQA, NA, MA)
    PROBAL = PROBAAr.tolist()
    
    PROBBAr = Bayesian_prob_from_quantile(QQA, NB, MB)
    PROBBL = PROBBAr.tolist()

    if supp:
        # Calculate and plot the Bayesian post. probability distribution (pdf)
        # and cumulative function (cdf)
        fname = "beta_pdf_" + str(NA) + '_' + str(MA)
        betapdfL = beta.pdf(PROBAAr, MA + 1, NA - MA + 1).tolist()
        u.make_r_jpeg(PROBAL, betapdfL, fname, "p", "betaPdf_A")
        u.show_jpeg(fname)

        fname = "beta_cdf_" + str(NA) + '_' + str(MA)
        betacdfL = beta.cdf(PROBAAr, MA + 1, NA - MA + 1).tolist()
        u.make_r_jpeg(PROBAL, betacdfL, fname, "p", "betaCdf_A")
        u.show_jpeg(fname)

        fname = "beta_pdf_" + str(NB) + '_' + str(MB)
        betapdfL = beta.pdf(PROBBAr, MB + 1, NB - MB + 1).tolist()
        u.make_r_jpeg(PROBBL, betapdfL, fname, "p", "betaPdf_B")
        u.show_jpeg(fname)

        fname = "beta_cdf_" + str(NB) + '_' + str(MB)
        betacdfL = beta.cdf(PROBBAr, MB + 1, NB - MB + 1).tolist()
        u.make_r_jpeg(PROBBL, betacdfL, fname, "p", "betaCdf_B")
        u.show_jpeg(fname)

    # Bayesian credibilities for 'simply better' and
    # default thresholds for each comparison metric
    BAYES_CRED = mc.probDiffM.calc_cred(NA, MA, NB, MB, PROBAAr, 0.)

    print "\n\nBayesian Credibility"
    print "Cred[A better than B] =", round(BAYES_CRED*100., 1), "%"

    ## default threshold = 1.5 X expected min return 
    for mtr in mtrCL:
        try:
            def_value = 1.5*minAccD[mtr.name]
            CRED_M = mtr.calc_cred(NA, MA, NB, MB, PROBAAr, def_value)
            print "Cred[",mtr.name, ">", def_value, "] =",\
                                round(100.*CRED_M, 1), "%"
        except KeyError:
            pass


    # Construct lists or arrays of comparison metrics
    # which span 98% of the volume, 
    # percentiles or values of the CDF
    percentileA = u.make_quantile_array(0.98, 99)


    ## Numerical calculation (Based on Bayesian distribution)
    ## of metric as a function of credibility

    # plot metric as a function of credibility (Bayesian)
    for mtr in mtrCL:
        mtr.plot_metric_vs_cred(NA, MA, NB, MB, PROBAAr, dataF)

    ## plot Minimum expected return as a function of credibility
    # find max(min expected return) and compare to min. acceptable value
    for mtr in mtrCL:
        if mtr.name in minAccD:
            maxmin = mtr.expected_min_return(NA, MA, NB, MB, PROBAAr, dataF)
            print "\nFor metric", mtr.name
            print "Exptl. Max(Exp Min. return) =", maxmin
            print "Min. Acceptable return =", minAccD[mtr.name]
            if minAccD[mtr.name] > maxmin:
                print "Variant NOT better than Control"
            else:
                print "Variant better than Control"
        else:
            print "\n", mtr.name, "not in config file"


    
if __name__ == "__main__":
    main()

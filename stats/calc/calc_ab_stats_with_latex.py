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

import json
import logging
# import numpy as np
from optparse import OptionParser, OptionGroup
import os
# import pandas as pd
import pdb
import sys

from math import sqrt, erf, exp
from scipy.stats import beta, norm
from subprocess import call, Popen

sys.path.insert(0, os.path.abspath('..'))

from lib import utilities as u
from lib import ab_stats_functions as fn

from metrics import metricsClass as mc


def validate_data(dataD):
    if len(dataD) not in (4, 6):
        print "Unexpected number of arguments, 4 or 6 expected, exiting"
        sys.exit(1)
    for data_name in ("NA", "MA", "NB", "MB"):
        data_value = dataD[data_name]
        if type(data_value) != type(1):
            print "Invalid data_type", data_name, data_value, "exiting"
            sys.exit(0)

    NA = dataD["NA"]
    MA = dataD["MA"]
    NB = dataD["NB"]
    MB = dataD["MB"]
    
    # Check validity of input values
    if (NA <= 0 or MA < 0 or NB <= 0 or MB < 0):
        print "Negative count encountered, exiting"
        sys.exit(0)

    if (MA > NA or MB > NB):
        print "Inequalities in counts not satisfied, exiting"
        sys.exit(0)

    return NA, MA, NB, MB
    

def get_variant_names(dataD):
    # optional Variant names
    if 'varA' in dataD.keys():
        varA = dataD["varA"]
    else:
        varA = "TestSegment"

    if 'varB' in dataD.keys():
        varB = dataD["varB"]
    else:
        varB = "Control"

    return varA, varB

def get_metric_names():
    mtrCL = mc.metricC.metricsL
    mnameL = map(lambda i: i.name, mtrCL) #metrics in class
    return mnameL
    

def get_metric_min_values(metricsD):
    mtrCL = mc.metricC.metricsL
    mnameL = get_metric_names()
    
    minD = {} # minimum acceptable return
    for mkey in metricsD.keys():
        mname = mkey.strip()
        if (mname in mnameL):
            if (type(metricsD[mkey]) == type(0.1)):
                minD[mname] = metricsD[mkey]
            else:
                print "Min. Acceptable Value for", mname, "not float"
                print "Continuing with rest"
        else:
            print "\nConfig Error: metric", mkey, "in", configF
            print mname, "not in metrics Class list"
            print "Continuing with rest"

    # check valid range for minimum acceptable value of metrics
    for mtr in mtrCL:
        minm = mtr.lower_bound
        maxm = mtr.upper_bound
        mname = mtr.name
        # print mname, minm, maxm
        try:
            if minm <= minD[mname] <= maxm:
                pass
            else:
                print mname, "Min. Acceptable value=", minD.pop(mname)
                print "out of range, continuing with rest"
        except KeyError:
            print mname, "not in config file"
            pass

    return minD


def initialize_output_directory(testname):
    # remove output files and (re)make directory
    outS = "./Output/" + testname + '/'
    rmS = "rm -fr " + outS
    mkS = "mkdir -p " + outS
    call(rmS.split(' '))
    call(mkS.split(' '))
    u.check_file(outS, "(re)made dir")
    return outS


def calc_freq_stats(NA, MA, NB, MB):
    # calculate the Frequentist descriptive stat.s
    # for both "A"=test and "B"=control
    # and the Frequentist confidence that A is better than B,
    # i.e. p_A > p_B
    MUA = u.sig_digits(fn.mu_freq(NA, MA), 3)
    VARMUA = u.sig_digits(fn.var_mu_freq(NA, MA), 2)
    MUB = u.sig_digits(fn.mu_freq(NB, MB), 3)
    VARMUB = u.sig_digits(fn.var_mu_freq(NB, MB), 2)
    ZPROP = u.sig_digits(fn.zlift(NA, MA, NB, MB, 0), 2)
    FREQ_CONF = u.sig_digits(fn.conf(ZPROP), 3)

    freqD = {'MUA': MUA, 'VARMUA': VARMUA, 'MUB': MUB, 'VARMUB': VARMUB, 'z': ZPROP, 'confidence': round(FREQ_CONF*100, 1)}
    return freqD


def get_Bayesian_pdf(p, trials, successes):
    N = trials
    M = successes
    return beta.pdf(p, M + 1, N - M + 1) 


def get_Bayesian_cdf(p, trials, successes):
    N = trials
    M = successes
    return beta.cdf(p, M + 1, N - M + 1) 


def calc_Bayes_stats(NA, MA, NB, MB, pAAr):
    # calculate the Bayesian descriptive stat.s
    # for both "A"=test and "B"=control
    # and the Bayesian Credibility that A is better than B,
    # i.e. p_A > p_B
    MUA = u.sig_digits(fn.mu_bayes(NA, MA), 3)
    VARMUA = u.sig_digits(fn.var_mu_bayes(NA, MA), 2)
    MUB = u.sig_digits(fn.mu_bayes(NB, MB), 3)
    VARMUB = u.sig_digits(fn.var_mu_bayes(NB, MB), 2)
    CRED = u.sig_digits(mc.probDiffM.calc_cred(NA, MA, NB, MB, pAAr, 0.), 3)

    bayesD = {'MUA': MUA, 'VARMUA': VARMUA, 'MUB': MUB, 'VARMUB': VARMUB, 'credibility': round(CRED*100, 1)}
    return bayesD


def get_Bayesian_prob_from_quantile(qq, trials, successes):
    """ For M successes from N trials, calculates the probability 
    at which the cumulative of the Bayesian Post. dist. has a 
    given quantile value qq
    PPF is the inverse of the CDF"""
    N = trials
    M = successes
    return beta.ppf(qq, M + 1, N - M + 1)


def get_Bayesian_probs_from_quantiles(quantile_range, num_quantiles, trials, successes):
    QQA = u.make_quantile_array(quantile_range, num_quantiles)
    return get_Bayesian_prob_from_quantile(QQA, trials, successes)
    
def setupLogging():
    abLogger = logging.getLogger()
    abLogger.setLevel(logging.NOTSET)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    
    fileLogger = logging.FileHandler('./ab.log') 
    fileLogger.setLevel(logging.NOTSET)
    fileLogger.setFormatter(formatter)
    abLogger.addHandler(fileLogger)

    """consoleLogger = logging.StreamHandler()
    consoleLogger.setLevel(logging.NOTSET)
    consoleLogger.setFormatter(formatter)
    abLogger.addHandler(consoleLogger)"""
    return abLogger

def getArgParser():
    usage = "usage: %prog [options]"
    parser = OptionParser(usage=usage)
    group = OptionGroup(parser, "Regular Usage")
    group.add_option("-j" , "--json", action="store", type='string',  dest='json',
            help='Path to JSON config file [default: %default]',
            default="ABConfig_rst.json")
    parser.add_option_group(group)
    group = OptionGroup(parser, "Command Line Usage")
    group.add_option("-l", "--list_metrics", action="store_true",
            dest="list_metrics",
            help='When flag is set just prints the list of valid metrics',
            default=False)
    group.add_option("-m", "--metric", action="store", dest="metric",
            help='A metric name (as returned by -l) '
            ' and threshold separated by a comma. Only evaluated when no JSON '
            'file is given'
            ' [default: %default]', default="")
    parser.add_option_group(group)
    return parser

def isDefault(parser, dest):
    if any (opt.dest == dest and (opt._long_opts[0] in sys.argv[1:] or
        opt._short_opts[0] in sys.argv[1:]) for opt in
        parser._get_all_options()):
            return True
    return False    

#def checkOptions(parser, options):
#    if options.json

def main(argv=None):
    
    #parser = getArgParser()
    #(options, args) = parser.parse_args()
    abLogger = setupLogging()
    f_path = os.getcwd()
    
    supp = 0 # do NOT produce supplementary figures for document or examination
    if len(sys.argv) == 2:
        cf_name = sys.argv[1]
        if cf_name.split('.')[-1] == 'json':
            configF = './jsons/' + cf_name
        else:
            configF = './jsons/' + cf_name+".json"
    else:
        print "Usage: "
        print "python calc_ab_stats.py config_file(.json)"
        print "See 'ABConfig_rst.json' for an example"
        print "No file or arg list found"
        print "Using 'ABConfig_rst.json' as default config file"
        configF = "./jsons/ABConfig_rst.json"

    # Read config File
    if os.path.exists(configF):
        f = open(configF, "r")
    else:
        abLogger.error("config file", configF, "not found, exiting")
        print "config file", configF, "not found, exiting"
        sys.exit(1)

    confD = json.load(f)
    f.close()

    supp = confD['supp']
    test_name = confD['outName']
    labelL = test_name.split('_')[1:] # List of label items
    labelS = '_'.join(labelL)
    
    dataD = confD['data']
    NA, MA, NB, MB = validate_data(dataD)
    varA, varB = get_variant_names(dataD)
    variantD = {}
    varL = ['A', 'B']
    variantD['A'] = {'name': varA}
    variantD['B'] = {'name': varB}
    abLogger.info("\nINPUT: A =%s B=%s" %(varA,varB))
    abLogger.info("NA, MA, NB, MB = %s %s %s %s\n"%( NA, MA, NB, MB))
    print "\nINPUT: A =", varA, "B =", varB
    print "NA, MA, NB, MB = ", NA, MA, NB, MB, '\n'
    variantD['A']['N'] = NA
    variantD['A']['M'] = MA
    variantD['B']['N'] = NB
    variantD['B']['M'] = MB


    minAccD = get_metric_min_values(confD['metrics'])
    for mtr in minAccD:
        abLogger.info("Min Acceptable return for %s = %s" %(mtr,
            str(minAccD[mtr])))
        print "Min. Acceptable return for", mtr, "= ", minAccD[mtr]

    outS = initialize_output_directory(test_name)

    # ### tex stuff till ###
    tex_templateF = "results_template.tex"
    tex_resultsF = outS + "results.tex"
    t = open(tex_templateF, 'r')
    r = open(tex_resultsF, 'w')
    tex_test_name = test_name.replace('_', '')
    for line in t:
        newline = line.replace('$testname', tex_test_name)
        r.write(newline)
    t.close()
    r.close()

    tex_analysisF = outS + "analysis.tex"
    a = open(tex_analysisF, 'w')
    a.write("\n\section{"+tex_test_name + "}\n")
    a.write("\subsection{Data}\n")
    a.write("See Table~\\ref{table:data"+tex_test_name+"} for the original data.\n")
    a.close()
    
    hdr1S = ''
    row1L = []
    trialS = 'Trials(N)'
    succS = 'Successes(M)'
    for var in varL:
        hdr1S += " & " + var + " = " + variantD[var]['name']
        trialS += ' & ' + str(int(u.sig_digits(variantD[var]['N'], 3))) 
        succS += ' & ' + str(int(u.sig_digits(variantD[var]['M'], 3))) 
    row1L.append(trialS)
    row1L.append(succS)
    u.make_tex_table(outS + "abdatatable", hdr1S, row1L)
    u.input_tex_table(tex_analysisF, "abdatatable", "Data for " + tex_test_name, "table:data"+tex_test_name)

    hdr2S = " Metric & Value & Credibility(\%) & Min. Acc. & Max Expected & " + variantD['A']['name'] + " \(>\) " + variantD['B']['name'] + "?"
    row2L = []
    ###
    
    # Output Dictionary
    outD = {}
    
    outD['Frequentist'] = calc_freq_stats(NA, MA, NB, MB)
    line = "\n\nFrequentist Confidence Level\n"
    line = line + "Conf[A better than B] ={!s} % ".format(outD['Frequentist']['confidence'])
    abLogger.info(line)

    print "\n\nFrequentist Confidence Level"
    print "Conf[A better than B] =", outD['Frequentist']['confidence'], "%"
    freqS = "Freq. & 0 & " + str(outD['Frequentist']['confidence']) + " & & & "

    """ Bayesian Posterior distribution and properties
    Construct lists or arrays of p_A and p_B which span 99.8% of the volume
    of the Bayesian Posterior distribution
    for plotting or for numerical integration
    999 quantiles from 0.001 to 0.999
    These arrays and lists are CONSTANT
    Though the values do depend on NA, MA etc,
    they are fixed for the duration of the execution
    and have 'forgotten' about NA, MA, so might as well pass them
    instead of recalculating them """
    
    PROBAAr = get_Bayesian_probs_from_quantiles(0.998, 999, NA, MA)
    PROBAL = PROBAAr.tolist()
    PROBBAr = get_Bayesian_probs_from_quantiles(0.998, 999, NB, MB)
    PROBBL = PROBBAr.tolist()


    # Calculate and plot the Bayesian post. probability distribution (pdf)
    # and cumulative function (cdf)
    outD['Bayesian'] = {}

    BayesianVarD = {}
    xhdr = 'True_Probability'
    pdfAL = get_Bayesian_pdf(PROBAAr, NA, MA).tolist()
    cdfAL = get_Bayesian_cdf(PROBAAr, NA, MA).tolist()
    pdfBL = get_Bayesian_pdf(PROBBAr, NB, MB).tolist()
    cdfBL = get_Bayesian_cdf(PROBBAr, NB, MB).tolist()
    PDFAD = {'list': pdfAL}
    CDFAD = {'list': cdfAL}
    PDFBD = {'list': pdfBL}
    CDFBD = {'list': cdfBL}
    BayesianVarD['A'] = {'xL': PROBAL, 'PDF': PDFAD, 'CDF': CDFAD}
    BayesianVarD['B'] = {'xL': PROBBL, 'PDF': PDFBD, 'CDF': CDFBD}
    for var in varL:
        for plot in ['PDF', 'CDF']:
            yhdr = 'Bayesian_' + plot
            oname = outS + variantD[var]['name'] + '_'
            fname = oname + yhdr + '_vs_' + xhdr
            u.write_csv(BayesianVarD[var]['xL'],\
                        BayesianVarD[var][plot]['list'], fname, xhdr, yhdr)
            BayesianVarD[var][plot]['csv'] = fname + '.csv'
            u.make_jpeg(BayesianVarD[var]['xL'],\
                        BayesianVarD[var][plot]['list'], oname,\
                        xhdr, yhdr, variantD[var]['name'])
            BayesianVarD[var][plot]['jpeg'] = fname + '.jpeg'

    if supp == 1:
        for var in BayesianVarD:
            for plot in ['PDF', 'CDF']:
                fname = BayesianVarD[var][plot]['jpeg']
                # pdb.set_trace()
                u.show_jpeg(fname)
                

    # Bayesian credibilities for 'simply better' and
    # default thresholds for each comparison metric
    outD['Bayesian']['plain vanilla'] = calc_Bayes_stats(NA, MA, NB, MB,PROBAAr)

    line = "\n\nBayesian Credibility\n"
    line += "Cred[A better than B] = {!s} %".format(outD['Bayesian']['plain vanilla']['credibility'])
    abLogger.info(line)
    print "\n\nBayesian Credibility"
    print "Cred[A better than B] =", outD['Bayesian']['plain vanilla']['credibility'], "%\n"
    BayesS = "Bayes & 0 & " + str(outD['Bayesian']['plain vanilla']['credibility']) + " & & & "
    row2L.append(BayesS)
    row2L.append(freqS)
    
    ## default threshold = 1.1 X Min. Acceptable Expected return 
    mtrCL = mc.metricC.metricsL
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            def_value = 1.1 * minAccD[mname]
            CRED_M = mtr.calc_cred(NA, MA, NB, MB, PROBAAr, def_value)
            line = "Cred[" + str(mname) + ">" + str(def_value) + "] =" +\
                                str(round(100.*CRED_M, 1)) +  "%"
            abLogger.info(line)
            print "Cred[", mname, ">", def_value, "] =",\
                                round(100.*CRED_M, 1), "%"
            outD['Bayesian'][mname] = {}
            outD['Bayesian'][mname]['value'] = def_value
            outD['Bayesian'][mname]['credibility'] = round(CRED_M*100., 1)
        else:
            print mname, "not in minAccD"
            abLogger.info(str(mname) + "not in minAccD")
        #print outD['Bayesian'][mname]


    # Construct lists or arrays of comparison metrics
    # which span 98% of the volume, 
    # percentiles or values of the CDF
    percentileA = u.make_quantile_array(0.98, 99)


    ## Numerical calculation (Based on Bayesian distribution)
    ## of credibility as a function of metric

    # plot credibility (Bayesian) as a function of metric
    plot = 'credibility_vs_metric'
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            xhdr = mname
            yhdr = 'Credibility'
            xL, yL = mtr.plot_cred_vs_metric(NA, MA, NB, MB, PROBAAr, outS, labelS)
            fname = outS + yhdr + '_vs_' + xhdr
            u.write_csv(xL, yL, fname, xhdr, yhdr)
            outD['Bayesian'][mname][plot] = {}
            outD['Bayesian'][mname][plot]['jpeg'] = fname + '.jpeg'
            outD['Bayesian'][mname][plot]['csv'] = fname + '.csv'
            if supp == 1:
                u.show_jpeg(outD['Bayesian'][mname][plot]['jpeg'])
        else:
            pass
        #print mname, outD['Bayesian'][mname]


    ## plot expected minimum return as a function of metric, (not credibility)
    # find max(expected minimum return) and compare to min. acceptable value
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            abLogger.info("\n\nFor metric " + mname)
            print "\n\nFor metric", mname
            maxmin, credL, mvalueL, exp_retL = mtr.plot_expected_return(NA, MA, NB, MB, PROBAAr, outS, labelS)
            yL = exp_retL
            yhdr = 'Expected_Return'
            fname = outS + yhdr + "_vs_" + mname
            u.write_csv(mvalueL, yL, fname, mname, yhdr)
            if supp == 1:
                u.show_jpeg(fname)
            plot = 'exp_ret_vs_metric'
            outD['Bayesian'][mname][plot] = {}
            outD['Bayesian'][mname][plot]['jpeg'] = fname + '.jpeg'
            outD['Bayesian'][mname][plot]['csv'] = fname + '.csv'
            """for plot in ['exp_ret_vs_cred', 'exp_ret_vs_metric']:
            if plot == 'exp_ret_vs_cred':
                fname = outS + yhdr + "_" + mname + "_vs_" + "Credibility"
            elif plot == 'exp_ret_vs_metric':
                fname = outS + yhdr + "_vs_" + mname
                if supp == 1:
                    u.show_jpeg(fname)
            else:
                print "Unknown plot type"
                abLogger.error("Unkown plot type")
            outD['Bayesian'][mname][plot] = fname + '.jpeg' """

            maxminS = "Experimental Max(Expected Return) = "+str(round(maxmin,3))
            minaccS = "Minimum Acceptable Return = " + str(minAccD[mname])
            if minAccD[mname] > maxmin:
                resultS = "NO"
            else:
                resultS = "YES"
            abLogger.info(maxminS)
            abLogger.info(minaccS)
            abLogger.info(maxminS)
            abLogger.info(resultS)
            print maxminS
            print minaccS
            print resultS
            outD['Bayesian'][mname]['maxmin'] = round(maxmin,3)
            outD['Bayesian'][mname]['minacc'] = minAccD[mname]
            outD['Bayesian'][mname]['result'] = resultS
            mS = mname
            mS += ' & ' + str(outD['Bayesian'][mname]['value'])
            mS += ' & ' + str(outD['Bayesian'][mname]['credibility'])
            mS += ' & ' + str(outD['Bayesian'][mname]['minacc'])
            mS += ' & ' + str(outD['Bayesian'][mname]['maxmin'])
            mS += ' & ' + outD['Bayesian'][mname]['result']
            row2L.append(mS)
            #print mname, outD['Bayesian'][mname]
        else:
            pass
    
    a = open(tex_analysisF, 'a')
    a.write("\subsection{Summary}\n")
    a.write("See Table~\\ref{table:results"+tex_test_name+"} for a summary of the results.\n")
    a.close()
    
    u.make_tex_table(outS + "abresulttable", hdr2S, row2L)
    u.input_tex_table(tex_analysisF, "abresulttable", "Results for "+ tex_test_name, "table:results"+tex_test_name)
    #print outD['Bayesian'].keys()
    #print minAccD.keys()
    
    for mname in outD['Bayesian']:
        if mname in ('test', 'control', 'plain vanilla'):
            pass
        else:
            f = open(tex_analysisF, 'a')
            f.write("\\clearpage\n")
            f.write('\subsection{'+mname+'}\n')
            f.write("See Fig.s~\\ref{fig:cred"+mname+"} and \\ref{fig:exp"+mname+"} for a plot of the Credibility and Expected return vs. the values of " + mname+".\n")
            f.close()
            #print mname, outD['Bayesian'][mname]
            figF = outD['Bayesian'][mname]['credibility_vs_metric']['jpeg'].split('/')[-1]
            u.input_tex_figure(tex_analysisF, figF, "Credibility vs. " + mname, "fig:cred"+mname)
            
            figF = outD['Bayesian'][mname]['exp_ret_vs_metric']['jpeg'].split('/')[-1]
            u.input_tex_figure(tex_analysisF, figF, "Expected Return vs. " + mname, "fig:exp"+mname)

            
    gname = outS+ 'all.json'
    g = open(gname, 'w')
    g.write(json.dumps(outD))
    g.close()
    print '\n\nWrote', gname, '\n'
    abLogger.info("\n\nWrote" + gname )
    
if __name__ == "__main__":
    main()

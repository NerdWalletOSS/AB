#!/usr/bin/python
""" ParametricCorrVsBeta.py
    without and with correcting for endpoints"""

import os
import sys
from subprocess import call
from math import erf, sqrt, exp, log
from scipy.stats import beta, norm
import numpy as np

def write_csv(xL, yL, f_name, xhdr, yhdr):
    if len(xL) == len(yL):
        pass
    else:
        print "unequal length lists, exiting"
        sys.exit(2)
    full_name = "./_"+f_name+".csv"
    f = open(full_name, 'w')
    f.write(xhdr + ', ' + yhdr)
    for i in range(len(xL)):
        f.write("\n"+str(xL[i]) + ', ' + str(yL[i]))
    f.close()
    #print "wrote", full_name

def write_Rscrp(f_name, xhdr, yhdr):
    RSName = "./_"+f_name+".R"
    csvName = "./_"+f_name+".csv"
    jpegName = "./_"+f_name+".jpeg"
    r = open(RSName, 'w')
    r.write('xydata <- read.csv("'+csvName+'")')
    r.write("\nattach(xydata)")
    r.write('\njpeg(filename = "'+jpegName+'")')
    r.write("\nplot("+xhdr+", "+yhdr+", type = 'b', main = '"+f_name+"')")
    r.write("\ndev.off()")
    r.close()
    #print "wrote", RSName

def runR(f_name):
    RSName = "_"+f_name+".R"
    #print RSName
    Rcmd = "R CMD BATCH " + RSName
    #print Rcmd
    call(Rcmd.split(" "))

def make_jpeg(xL, yL, f_name="xydata", xhdr="x", yhdr="y"):
    write_csv(xL, yL, f_name, xhdr, yhdr)
    write_Rscrp(f_name, xhdr, yhdr)
    runR(f_name)
    jpegName = "./_"+f_name+".jpeg"
    #print "Made ", jpegName

def show_jpeg(f_name):
    jpegName = "./_"+f_name+".jpeg"
    show = "xdg-open " + jpegName
    # Tried "eog " + jpegname + " &", worked once
    # then stopped working, xdg-open redirects to default viewer, which is eog!
    # seems to work!"""
    call(show.split(" "))

def muB(n,m):
    """ Calculates the mean of the distribution of the 
    Bayesian prior probabilities
    Arguments: number of trials, number of successes
    """
    return (m+1.)/(n+2.)

def var_muB(n,m):
    """ Calculates the variance of the distribution of the 
    Bayesian prior probabilities
    Arguments: number of trials, number of successes
    """
    return muB(n,m)*(1.-muB(n,m))/(n+3.)

def normal_cdf(p, n, m):
    mean = muB(n,m)
    sigma = sqrt(var_muB(n,m))
    d = norm(loc = mean, scale = sigma)
    return d.cdf(p)

def normal_cdfCorr(p, n, m):
    mean = muB(n,m)
    sigma = sqrt(var_muB(n,m))
    d = norm(loc = mean, scale = sigma)
    cdfp = d.cdf(p)
    cdf0 = d.cdf(0)
    cdf1 = d.cdf(1)
    return (cdfp-cdf0)/(cdf1-cdf0)

def odds_p(p):
    p_bd = min(1. - exp(-20), max(exp(-20), p))
    return p_bd/(1. -p_bd)

def p_odds(o):
    if o > 0:
        pass
    else:
        print "!(o > 0), exiting"
        sys.exit(2)
    return o/(1.+o)

def rel_odds(pa, pb):
    return odds_p(pa)/odds_p(pb)

def rel_odds_norm_beta(p, n, m):
    norm_cdf = normal_cdf(p,n,m)
    beta_cdf = beta.cdf(p, m+1, n-m+1)
    return rel_odds(norm_cdf, beta_cdf)
        
def rel_odds_normCorr_beta(p, n, m):
    norm_cdfCorr = normal_cdfCorr(p,n,m)
    beta_cdf = beta.cdf(p, m+1, n-m+1)
    return rel_odds(norm_cdfCorr, beta_cdf)        


def main():
    print sys.argv[0]

    x_hdr = "p"
    for n in [40,100]:
        if n == 40:
            mL = [4,10,20]
        elif n ==100:
            mL = [10, 15, 20]
        print "\n\nCum. Fn. and rel. odds for various n, m, Vs."+x_hdr
        
        for m in mL:
            print "\nn,m = ", n,m
            mean = muB(n,m)
            sigma = sqrt(var_muB(n,m))
            pA = beta.ppf(np.array([0.01 + i/100. for i in range(99)])
                           , m+1, n-m+1)
            pL = pA.tolist()
            print "\nn, m, mean, sigma, p(1%), p(99%) = ", n, m,\
                round(mean, 3), round(sigma, 4),\
                round(pL[0],3), round(pL[-1], 4)

            y_hdr = "betaCumFn"
            fname = "betaCF_n"+str(n) + '_m'+str(m)
            betacdfL = beta.cdf(pA, m+1, n-m+1).tolist()
            make_jpeg(pL, betacdfL, fname, x_hdr, y_hdr)
            show_jpeg(fname)
            print "betaCDF at 1% and 99%", betacdfL[0], betacdfL[-1]
            
            y_hdr = "NormalCumFn"
            fname = "normCF_n"+str(n) + '_m'+str(m)
            normcdfL = [normal_cdf(p,n,m) for p in pL]
            make_jpeg(pL, normcdfL, fname, x_hdr, y_hdr)
            show_jpeg(fname)
            print "normalCDF at 1% and 99%", round(normcdfL[0],4),\
                round(normcdfL[-1], 4)
            print "normalCDF at p=0,1", round(normal_cdf(0,n,m),4),\
                round(normal_cdf(1,n,m), 4)
            
            y_hdr = "NormalCorrCumFn"
            fname = "normCorrCF_n"+str(n) + '_m'+str(m)
            normCorrcdfL = [normal_cdfCorr(p,n,m) for p in pL]
            make_jpeg(pL, normCorrcdfL, fname, x_hdr, y_hdr)
            show_jpeg(fname)
            print "normalCorrCDF at 1% and 99%", round(normCorrcdfL[0],4),\
                round(normCorrcdfL[-1],4)

            y_hdr = "abs_log_rel_odds_Norm_beta"
            abs_log_rel_oddsL = [abs(log(rel_odds_norm_beta(p, n, m)))
                                 for p in pL]
            f_name = "NormalVsBeta_n"+str(n)+"_m"+str(m)
            make_jpeg(pL, abs_log_rel_oddsL, f_name, x_hdr, y_hdr)
            show_jpeg(f_name)
            print "normalVsbeta_relodds at 1% and 99%",\
                round(abs_log_rel_oddsL[0],4), round(abs_log_rel_oddsL[-1],4)

            y_hdr = "abs_log_rel_odds_NormCorr_beta"
            abs_log_rel_oddsL = [abs(log(rel_odds_normCorr_beta(p, n, m)))
                                 for p in pL]
            f_name = "NormalCorrVsBeta_n"+str(n)+"_m"+str(m)
            make_jpeg(pL, abs_log_rel_oddsL, f_name, x_hdr, y_hdr)
            show_jpeg(f_name)
            print "normalCorrVsbeta_relodds at 1% and 99%",\
                round(abs_log_rel_oddsL[0],4), round(abs_log_rel_oddsL[-1],4)





if __name__ == "__main__":
    main()

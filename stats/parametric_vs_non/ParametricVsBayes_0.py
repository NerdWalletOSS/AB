#!/usr/bin/python
"""ParametricVsBayes.py"""

import os
import sys
from subprocess import call
from math import erf, sqrt, exp, log
from scipy.stats import beta, norm


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
    print "Made ", jpegName

def show_jpeg(f_name):
    jpegName = "./_"+f_name+".jpeg"
    show = "xdg-open " + jpegName
    # Tried "eog " + jpegname + " &", worked once
    # then stopped working, xdg-open redirects to default viewer, which is eog!
    # seems to work!"""
    call(show.split(" "))

def normal_cdf(x, mu, sigma):
    d = norm(loc = mu, scale = sigma)
    return d.cdf(x)

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

def odds_p(p):
    p_bd = min(1. - exp(-20), max(exp(-20), p))
    """if 0 < p < 1:
        pass
    else:
        print p, "!(0 < p < 1), exiting"
        sys.exit(2) """
    return p_bd/(1.-p_bd)

def p_odds(o):
    if o > 0:
        pass
    else:
        print "!(o > 0), exiting"
        sys.exit(2)
    return o/(1.+o)

def rel_odds(mua, mub):
    return odds_p(mua)/odds_p(mub)

def mu_c(mu, x): # x = odds factor
    return p_odds(x*odds_p(mu))

def z_mu(n, m, x):
    #print n,m,x
    #print mu_B(n,m)
    #print mu_c(mu_B(n,m),x)
    #print var_B(n,m)
    #print sqrt(var_B(n,m))
    return (mu_c(muB(n,m),x) - muB(n,m))/sqrt(var_muB(n,m))

def Phi(z):
    """ 1- Cum. function of the Normal distribution or 
        One sided confidence (lower bound)
        1- Phi = p-value
        """
    return norm.cdf(z)

def PhiCorr(p, mu, sigma):
    z = (p-mu)/sigma
    cdf_corr = ((norm.cdf(z) - norm.cdf(-mu/sigma))/
                (norm.cdf((1-mu)/sigma) - norm.cdf(-mu/sigma)))
    return 1- cdf_corr

def gamma_B(n,m,x): # Bayesian one sided confidence (lower bound)
    """ 1- Cum. function of the Bayesian prior distribution or 
        One sided confidence (lower bound)"""
    return 1. - beta.cdf(mu_c(muB(n,m), x), m+1, n-m+1)

def rel_odds(n,m,x):
    """print "\n", n,m,x
    print muB(n,m), sqrt(var_muB(n,m))
    print mu_c(muB(n,m), x)
    print beta.cdf(mu_c(muB(n,m), x), m+1, n-m+1)
    print gamma_B(n,m,x), odds_p(gamma_B(n,m,x))"""
    mu = muB(n,m)
    sigma = sqrt(var_muB(n,m))
    muc = mu_c(mu, x)
    #print PhiCorr(muc, mu, sigma), odds_p(PhiCorr(muc, mu, sigma))
    return odds_p(gamma_B(n,m,x))/odds_p(PhiCorr(muc, mu, sigma))


def main():

    x_hdr = "odds_factor"
    y_hdr = "rel_odds"
    n = 400
    print "\n"+y_hdr+"Vs."+x_hdr, "; n fixed = ", n
    for m in [5,10,200]:
        mu = muB(n,m)
        sigma = sqrt(var_muB(n, m))
        mu_min = max(10**(-10.), mu - 3*sigma) 
        mu_max = min(1-10**(-10.), mu + 3*sigma)
        print mu, sigma, mu_min, mu_max
        muL = [mu_min + (mu_max - mu_min)*i/100. for i in range(101)]

        fname = "beta_cdf_n"+str(n) + '_m'+str(m)
        betacdfL = map(lambda p : beta.cdf(p, m+1, n-m+1), muL)
        make_jpeg(muL, betacdfL, fname, "mu", "betaCdf")
        show_jpeg(fname)

        fname = "norm_cdf_n"+str(n) + '_m'+str(m)
        normcdfL = map(lambda p : normal_cdf(p, mu, sigma), muL)
        make_jpeg(muL, normcdfL, fname, "mu", "normCdf")
        show_jpeg(fname)

        min_logx = max(-1., log(odds_p(mu_min)/odds_p(mu)))
        max_logx = min(1., log(odds_p(mu_max)/odds_p(mu)))
        xL = [exp(min_logx + (max_logx - min_logx)*i/100.) for i in range(101)]
        #print xL
        rel_oddsL = [rel_odds(n, m, x) for x in xL]
        f_name = "BayesVsParam_n"+str(n)+"_m"+str(m)
        make_jpeg(xL, rel_oddsL, f_name, x_hdr, y_hdr)
        show_jpeg(f_name)


    x_hdr = "m"
    y_hdr = "rel_odds"
    n = 100
    print "\n"+y_hdr+"Vs."+x_hdr, "; n fixed = ", n
    for x in [0.5, 0.8, 1.]:
        mL = range(101)
        #print mL
        rel_oddsL = [rel_odds(n, m, x) for m in mL]
        f_name = "BayesVsParam_n"+str(n)+"_100x"+str(int(100*x))
        make_jpeg(mL, rel_oddsL, f_name, x_hdr, y_hdr)        
        show_jpeg(f_name)


    x_hdr = "n"
    y_hdr = "rel_odds"
    m = 10
    print "\n"+y_hdr+"Vs."+x_hdr, "; m fixed = ", m
    for x in [0.5, 0.8, 1.]:
        nL = [20 + n*4 for n in range(101)]
        #print nL
        rel_oddsL = [rel_odds(n, m, x) for n in nL]
        f_name = "BayesVsParam_m"+str(m)+"_100x"+str(int(100*x))
        make_jpeg(nL, rel_oddsL, f_name, x_hdr, y_hdr)        
        show_jpeg(f_name)


    x_hdr = "n"
    y_hdr = "rel_odds"
    mu = 0.10
    print "\n"+y_hdr+"Vs."+x_hdr, "; mu fixed = ", mu
    for x in [0.5, 0.8, 1.]:
        nL = [10*2**n for n in range(11)]
        #print nL
        rel_oddsL = [rel_odds(n, int(round(mu*n, 0)), x) for n in nL]
        f_name = "BayesVsParam_10mu"+str(1)+"_100x"+str(int(100*x))
        make_jpeg(nL, rel_oddsL, f_name, x_hdr, y_hdr)        
        show_jpeg(f_name)



if __name__ == "__main__":
    main()

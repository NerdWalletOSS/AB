#!/usr/bin/python                                                               

import numpy as np
import os
import sys

from math import sqrt, exp
from scipy.stats import beta, norm

sys.path.insert(0, os.path.abspath('..'))

from lib import ab_stats_functions as fns
from lib import utilities as u


class metricC:
    "Base class for all comparison metrics for BINARY A/B testing"
    metricsCount = 0
    metricsL = []
    
    def __init__(self, name, minim = 0., maxim = exp(23)):
        self.name = name # name of metric
        self.lower_bound = minim # metric lower bound
        self.upper_bound = maxim # metric upper bound
        metricC.metricsCount += 1 # counts instances of metricClass
        metricC.metricsL.append(self) # lists instances of metricClass

        
    def fn(self, x): # useful for different metrics
        return x

    def invfn(self, fx): # useful for different metrics
        return fx

    
    def descBayesStats(self, na, ma, nb, mb): ## depends on metric instance
        # Descriptive stats for the posterior Bayesian distribution
        # calculates the mean and var_mean for the two ind. dimensions
        # For the comparison metric
        # returns the approximate parameters mean(metric) and var(mean_metric)
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)

        ## depends on metric instance
        mean = mua - mub 
        var_mean = var_mua + var_mub

        return mean, var_mean


    def list_mvalues(self, na, ma, nb, mb): # universal class method
        # Approximates metric value for a quantile using
        # Normal approximation to actual posterior distribution of metric
        # using the approximate descriptive parameters
        # returns metric value list for 1-99%iles
        percentileA = u.make_quantile_array(0.98, 99)
        MEAN, VAR = self.descBayesStats(na, ma, nb, mb)
        SE = sqrt(VAR)
        dist = norm(loc = MEAN, scale = SE)
        mvalueA = dist.ppf(percentileA)
        lb = self.lower_bound
        ub = self.upper_bound
        mvalueL = [max(lb, mv) for mv in [min(ub, mvp) for mvp in mvalueA]] 
        return mvalueL

    
    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([max(0.0, p) for p in [min(1.0, pb - mvalue)
                                               for pb in pAAr]])
        return pBAr


    def calc_cred(self, na, ma, nb, mb, paA, mvalue): # universal class method
        """ Calculates credibility for metric > mvalue, given 
        Args:
            trials and successes for variants A and B
            and the array of probabilities for 99.8% of 
            the Bayesian Post. prob. dist. for variant A
        Returns:
            prob density weighted area under the metric curve
        """
        # values of the distribution for post. prob.s in array for "variant A"
        betapdfaA = beta.pdf(paA, ma+1, na-ma+1)
        dpaA = u.first_diff(paA) # interval at pa
        pbA = self.make_curve(paA, mvalue)
        # incomplete integral along pB upto curve
        inc_betacdfbA = beta.cdf(pbA, mb + 1, nb - mb + 1) 
        cred = np.sum(np.multiply(
            np.multiply(betapdfaA, inc_betacdfbA), dpaA))
        return cred


    def calc_cred_vs_metric(self, na, ma, nb, mb, pAAr):
        # universal class method
        ## Numerical calculation (Based on Bayesian distribution)
        ## of metric as a function of credibility %
        mvalueL = self.list_mvalues(na, ma, nb, mb)
        credL = [100. * self.calc_cred(na, ma, nb, mb, pAAr, mvalue)
                         for mvalue in mvalueL]
        return mvalueL, credL


    def plot_cred_vs_metric(self, na, ma, nb, mb, pAAr, outName, pltlbl):
        # universal class method
        mvalueL, credL = self.calc_cred_vs_metric(na, ma, nb, mb, pAAr)
        mtrname = self.name
        u.make_jpeg(mvalueL, credL, outName, mtrname, "Credibility", pltlbl)
        return mvalueL, credL


    def plot_metric_vs_cred(self, na, ma, nb, mb, pAAr, outName, pltlbl):
        # universal class method
        mvalueL, credL = self.calc_cred_vs_metric(na, ma, nb, mb, pAAr)
        mtrname = self.name
        u.make_jpeg(credL, mvalueL, outName, "Credibility", mtrname, pltlbl)
        return credL, mvalueL


    def return_from_metric(self, mvAr): # depends on metric
        ## min return as a function of metric threshold
        return mvAr

    def metric_from_return(self, ret): # depends on metric
        # inverse function of return_from_metric
        return ret
    
    def plot_expected_return(self, na, ma, nb, mb, pAAr, outName, pltlbl): 
        # universal class method
        # cred * return(metric_threshold) as a function of metric_threshold
        mvalueL, credL = self.calc_cred_vs_metric(na, ma, nb, mb, pAAr)
        mvalueA = np.array(mvalueL)
        credA = np.array(credL)
        returnA = self.return_from_metric(mvalueA)
        expected_returnL = (returnA * credA/100.).tolist()
        mtrname = self.name
        yhdr = "Expected_Return_" + mtrname
        u.make_jpeg(credL, expected_returnL, outName, "Credibility", yhdr, pltlbl)
        yhdr = "Expected_Return"
        u.make_jpeg(mvalueL, expected_returnL, outName, mtrname, yhdr, pltlbl)
        return self.metric_from_return(max(expected_returnL))\
            , credL, mvalueL, expected_returnL 

        
class probMetricC(metricC):
    pass


probDiffM = probMetricC('ProbDiff', -1.0, 1.0)


class probLiftC(probMetricC):
    def descBayesStats(self, na, ma, nb, mb): ## depends on metric instance
        # Descriptive stats for the posterior Bayesian distribution
        # calculates the mean and var_mean for the two ind. dimensions
        # For the comparison metric
        # returns the approximate parameters mean(metric) and var(mean_metric)
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        mean = mua/mub - 1 + var_mub*mua/mub**3 
        var_mean = var_mua/mub**2 + var_mub * mua**2/mub**4
        return mean, var_mean

    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([max(0.0, p) for p in [min(1.0, pb)
                                               for pb in pAAr / (1. + mvalue)]])
        return pBAr

    
probLiftM = probLiftC('ProbLift', -1.0)


class oddsMetricsC(metricC):
    def fn(self, x): 
        return fns.odds_from_p(x)
    
    def invfn(self, fx): 
        return fns.p_from_odds(fx)

    
class oddsFactorC(oddsMetricsC):
    def descBayesStats(self, na, ma, nb, mb):
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        oddsa = self.fn(mua)
        oddsb = self.fn(mub)
        mean = oddsa/oddsb + (var_mua/(oddsb * (1- mua)**3) +
                                  var_mub * oddsa/mub**3)
        var_mean = var_mua/(oddsb**2 * (1 - mua)**4) + var_mub * oddsa**2/mub**4
        return mean, var_mean

    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([self.invfn(self.fn(pa) / mvalue) for pa in pAAr])
        return pBAr
    
    def return_from_metric(self, mvAr): # depends on metric
        ## min return as a function of metric threshold
        return mvAr - 1.

    def metric_from_return(self, ret): # depends on metric
        # inverse function of return_from_metric
        return ret + 1.
    

oddsFactorM = oddsFactorC('OddsFactor')


"""************************
"""
class pageViewsLiftC(oddsMetricsC):
    def descBayesStats(self, na, ma, nb, mb):
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        oddsa = self.fn(mua)
        oddsb = self.fn(mub)
        mean = (oddsa/oddsb - 1) + (var_mua/(oddsb * (1- mua)**3) +
                                  var_mub * oddsa/mub**3)
        var_mean = var_mua/(oddsb**2 * (1 - mua)**4) + var_mub * oddsa**2/mub**4
        return mean, var_mean

    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([self.invfn(self.fn(pa)/(1. + mvalue)) for pa in pAAr])
        return pBAr
        

pageViewsLiftM = pageViewsLiftC('PageViewsLift', -1.0)


"""**************************
"""

class pageViewsDiffC(oddsMetricsC): # or previously defined subClass
    
    def descBayesStats(self, na, ma, nb, mb): ## depends on metric instance
        # Descriptive stats for the posterior Bayesian distribution
        # calculates the mean and var_mean for the two ind. dimensions
        # For the comparison metric
        # returns the approximate parameters mean(metric) and var(mean_metric)
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        oddsa = self.fn(mua)
        oddsb = self.fn(mub)
        mean = (oddsa - oddsb) + (var_mua/(1 - mua)**3 + var_mub/(1 - mub)**3)
        var_mean = var_mua/(1 - mua)**4 + var_mub/(1 - mub)**4
        return mean, var_mean


    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([self.invfn(max(exp(-23), self.fn(pa) - mvalue)) for pa in pAAr])
        return pBAr


pageViewsDiffM = pageViewsDiffC('PageViewsDiff', -exp(23), exp(23))

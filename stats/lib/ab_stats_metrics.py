#!/usr/bin/python                                                               
""" ab_stats_metrics.py
    DELTA = pA - pB (= 0.1)
    LIFT = pA/pB - 1 (= 0.2)
    ODDS_FACTOR = pA/(1-pA)/(pB/(1-pB)) (= 1.2)
    DELTA_VISITS = 1/(1-pA) - 1/(1-pB) (0.05)
    LIFT_VISITS = (1-pB)/(1-pA) - 1 (= 0.2)
"""

import numpy as np
import sys

from math import sqrt
from scipy.stats import beta

import utilities as u
import ab_stats_functions as fn


def descBayesStats(na, ma, nb, mb, metric):
    """ For the results of a binary A/B test, 
    We know the metric can't be normally distributed, but we 
    use the normal approx to the 2D Bayesian Posterior prob. dist. 
    to find good ranges of the non-linear metrics.
    Since we are assuming normality, there are no bounds on the metrics, 
    even if the values are meaningless.  
    
    Args:
        na, ma, nb, mb (trials and successes for a and b)
        for the specified metric ('Delta', 'Lift', 'OddsFactor')
        
    Calculates and returns:
        mean(metric), variance(metric)

    """
    mua = fn.mu_bayes(na, ma)
    var_mua = fn.var_mu_bayes(na, ma)
    mub = fn.mu_bayes(nb, mb)
    var_mub = fn.var_mu_bayes(nb, mb)

    if metric == "Delta": # = x - y
        mean = mua - mub
        var = var_mua + var_mub

    elif metric == "Lift": # = x/y - 1
        mean = mua/mub - 1 + var_mub*mua/mub**3 
        var = var_mua/mub**2 + var_mub * mua**2/mub**4

    elif metric == "OddsFactor": # = o(x)/o(y)
        oddsa = fn.odds_from_p(mua)
        oddsb = fn.odds_from_p(mub)
        mean = oddsa/oddsb + (var_mua/(oddsb * (1- mua)**3) +
                                  var_mub * oddsa/mub**3)
        var = var_mua/(oddsb**2 * (1 - mua)**4) + var_mub * oddsa**2/mub**4

    else:
        print "metric not in ('Delta', 'Lift', 'OddsFactor'), exiting"
        sys.exit(1)
    return mean, var

def make_boundary(pA, metric, threshold_value):
    """ The comparison metric is a function of (pA and pB)
    A value for the metric defines a curve in (pA, pB) space
    specified as pB = pB(pA).

    Args:
        Array of points corresponding to values of pA
        Choice of comparison metric
        Threshold or minimum important value of the metric.

    Returns:
        Array of pB values 
        corresponding to the points on the curve for each pA
    """
    if metric == "Delta":
        delta = threshold_value
        bdryA = np.array([min(1.0, pb) for pb in (pA - delta)])
    elif metric == "Lift":
        lift = threshold_value
        bdryA = np.array([min(1.0, pb) for pb in pA / (1. + lift)])
    elif metric == "OddsFactor":
        odds_factor = threshold_value
        bdryA = np.array([fn.p_from_odds(fn.odds_from_p(pa) / odds_factor)
                            for pa in pA])
    elif metric == "Page Visits Delta":
        pv_delta = threshold_value
        bdryA = np.array([fn.p_from_pagevisits(fn.pagevisits_from_p(pa) -\
                                               pv_delta) for pa in pA])
    elif metric == "Page Visits Lift":
        pv_lift = threshold_value
        bdryA = np.array([fn.p_from_pagevisits(fn.pagevisits_from_p(pa)/\
                                               (1 + pv_lift)) for pa in pA])
    else:
        print "metric not in ('Delta', 'Lift', 'OddsFactor'), exiting"
        sys.exit(1)
    return bdryA

def calc_cred(na, ma, nb, mb, metric, threshold_value, paA):
    """ Calculates credibility for metric > threshold_value, given 

    Args:
        trials and successes for variants A and B
        and the array of probabilities for 99.8% of 
        the Bayesian Post. prob. dist. for variant A
        
    Returns:
        prob density weighted area under the metric curve
    """
    # values of the distribution for post. prob.s in array for "variant A"
    betapdfA = beta.pdf(paA, ma+1, na-ma+1)
    dpaA = u.first_diff(paA) # interval at pa
    boundaryA = make_boundary(paA, metric, threshold_value)
    # incomplete integral along pB upto boundary
    inc_betacdf_B = beta.cdf(boundaryA, mb + 1, nb - mb + 1) 
    cred = np.sum(np.multiply(
        np.multiply(betapdfA, inc_betacdf_B), dpaA))
    return cred
    


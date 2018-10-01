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


def z_proposition(na, ma, nb, mb, metric, threshold_value):
    """ For the results of a binary A/B test, 
    We know the metric can't be normally distributed, but we make 
    the assumption of normality and use the z-value
    for parametric confidence levels etc.
    Since we are assuming normality, there are no bounds on the metrics, 
    even if the values are meaningless.  
    All are normal approximations to the Bayesian Post. prob. distribution
    
    Args:
        na, ma, nb, mb (trials and successes for a and b)
        for the specified metric ('Delta', 'Lift', 'Odds Factor',
        'Page Visits Delta', 'Page Visits Lift')
        the proposition is that the metric exceeds the threshold value.
        
    Calculates:
        mean(metric), variance(metric)

    Returns:
        the z-value as mean/Standard Error
    """    
    mua = fn.mu_freq(na, ma)
    var_mua = fn.var_mu_freq(na, ma)
    mub = fn.mu_freq(nb, mb)
    var_mub = fn.var_mu_freq(nb, mb)

    if metric == "Delta":
        delta = threshold_value
        mean_prop = mua - (mub + delta)
        var_prop = var_mua + var_mub

    elif metric == "Lift":
        lift = threshold_value
        mean_prop = mua - (1 + lift) * mub 
        var_prop = var_mua + (1 + lift)**2 * var_mub

    elif metric == "Odds Factor": # prop = odds_A - factor * odds_B
        factor = threshold_value
        mean_prop = fn.odds_from_p(mua) +var_mua / (1 - mua)**3 -\
                    - factor * (fn.odds_from_p(mub) + var_mub / (1 - mub)**3) 
        var_prop = var_mua/(1 - mua)**4 + factor**2 * var_mub/(1 - mub)**4

    elif metric == "Page Visits Delta": # prop = visits_A - visits_B - delta_v
        factor = threshold_value
        mean_prop = fn.pagevisits_from_p(mua) + var_mua / (1 - mua)**3 -\
                (fn.pagevisits_from_p(mub) + var_mub / (1 - mub)**3) - factor 
        var_prop = var_mua/(1 - mua)**4 + var_mub/(1 - mub)**4

    elif metric == "Page Visits Lift": # prop = visits_A - visits_B(1+lambda_v)
        factor = threshold_value
        mean_prop = (1 - mub) - (1 + factor) * (1 - mua)
        var_prop = var_mub + (1 + factor)**2 * var_mua

    else:
        print "metric not in ('Delta', 'Lift', 'Odds Factor' ..."
        print ", 'Page Visits Delta', 'Page Visits Lift'), exiting"
        sys.exit(1)
    return mean_prop / sqrt(var_prop)

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
        bdryA = np.array([min(1.0, pb) for pb in pA / (1.0 + lift)])
    elif metric == "Odds Factor":
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
        print "metric not in ('Delta', 'Lift', 'Odds Factor' ..."
        print ", 'Page Visits Delta', 'Page Visits Lift'), exiting"
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
    


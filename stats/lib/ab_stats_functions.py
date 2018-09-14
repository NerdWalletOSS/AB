#!/usr/bin/python                                                               
""" ab_stats_functions.py
"""

import sys

from scipy.stats import norm
from math import sqrt


def mu_bayes(n, m):
    """ Calculates the mean of the distribution of the 
    Bayesian post. probabilities

    Args:
        number of trials, number of successes

    Returns:
        mean of beta.pdf(m + 1, n - m + 1)
    """
    return (float(m) + 1) / (n + 2)

def var_mu_bayes(n, m):
    """ Calculates the variance of the distribution of the 
    Bayesian post. probabilities

    Args:
        number of trials, number of successes

    Returns:
        variance of beta.pdf(m + 1, n - m + 1)
   """
    return mu_bayes(n, m) * (1 - mu_bayes(n, m)) / (n + 3)

def mu_freq(n, m):
    """ Calculates the Frequentist mean probability

    Args:
        number of trials, number of successes

    Returns:
        mean probability
    """
    return float(m) / n

def var_mu_freq(n, m):
    """ Calculates the Frequentist variance of the mean probability

    Args:
        number of trials, number of successes

    Returns:
        variance of the mean probability
   """
    return mu_freq(n, m) * (1 - mu_freq(n, m)) / (n - 1)

def zlift(na, ma, nb, mb, lift):
    mua = mu_freq(na, ma)
    varmua = var_mu_freq(na, ma)
    mub = mu_freq(nb, mb)
    varmub = var_mu_freq(nb, mb)
    return (mua - mub * (1+lift))/sqrt(varmua + varmub * (1+lift)**2)

def conf(z):
    return norm.cdf(z)

def odds_from_p(p):
    if not(0 < p < 1):
        print "!(0 < p < 1), exiting"
        sys.exit(1)
    return p / (1.0 - p)

def p_from_odds(o):
    if not(o > 0):
        print "!(o > 0), exiting"
        sys.exit(1)
    return o / (1.0 + o)

def rel_odds(mua, mub):
    return odds_from_p(mua) / odds_from_p(mub)


def meanpageviews_from_prob(p):
    if not(0 < p < 1):
        print "!(0 < p < 1), exiting"
        sys.exit(1)
    return 1 / (1.0 - p)

def prob_from_meanpageviews(v):
    if not(v > 0):
        print "!(v > 0), exiting"
        sys.exit(1)
    return 1.0 - 1/v


#!/usr/bin/python
""" stats_modules.py
    test data in test_data.csv
    descriptive stat.s for testing in test_stats.csv
"""
import os, sys, pickle, codecs

# modules go here
def assertEqual(a,b):
    if a == b:
        print "Pass"
    else :
        print "Fail"

def mean(dataL, wtL=None):
    """ Calculates the weighted mean of the data. The list of weights is optional.
        If no weights are given, they are assumed to be uniform [1, 1, 1,  ...] """
    if type(dataL) == type([1]):
        pass
    else:
        print "1st argument to mean() not list, see help(mean)"
        sys.exit(2)
    if wtL == None:
        wtL = map(lambda i : 1, range(len(dataL)))
    elif type(wtL) == type([1]):
        if len(wtL) == len(dataL):
            pass
        else:
            print "arguments to wted_mean() unequal length"
            sys.exit(2)
    else:
        print "2nd argument to wted_mean() not list, see help(mean)"
        sys.exit(2)
    wdataL = map(lambda i: dataL[i]*wtL[i], range(len(dataL)))
    return 1.*sum(wdataL)/sum(wtL)

def cov_p(x1L, x2L=None, wtL=None):
    """Weighted population covariance, the second variable and the weights are optional
       Safest usage = keyword for arguments:
       cov_p(x1L = <x1L>, x2L=<x2L>|None, wtL = <wtL>|None)
       Next Safest usage is positional with use of None
       Special cases, simplest use:
       1. weighted covariance, positional arguments:
       cov_p(<x1L>, <x2L>, <wtL>)
       2. (unweighted) covariance, positional arguments:
       cov_p(<x1L>, <x2L>)
       3. weighted variance, positional - keyword arguments:
       cov_p(<x1L>, <x1L>, <wtL>),
       cov_p(<x1L>, None, <wtL>),
       cov_p(<x1L>, wtL = <wtL>)
       4. (unweighted) variance:
       cov_p(<x1L>) or cov_p(<x1L>, <x1L>)
 """
    if type(x1L) == type([1]):
        n = len(x1L)
    else:
        print "1st argument to cov_p() not list"
        sys.exit(2)
    if x2L == None :
        x2L = x1L
    elif type(x2L) == type([1]):
        if len(x2L) == n:
            pass
        else:
            print "1st and 2nd arguments to cov_p() not equal length" 
    else:
        print "2nd argument to cov_p() not list"
        sys.exit(2)
    if wtL == None:
        wtL = [1 for i in range(n)]
    elif type(wtL) == type([1]):
        if len(wtL) == n:
            pass
        else:
            print "weights list not same length as data"
    else:
        print "weights argument to cov_p() not list"
        sys.exit(2)
    prod_dataL = [x1L[i]*x2L[i] for i in range(n)]
    return mean(prod_dataL, wtL) - mean(x1L, wtL)*mean(x2L, wtL)

def main():
    if len(sys.argv) != 1: 
        print "usage $ stats_modules.py "
        sys.exit(2)
    else:
        pass # fname = sys.argv[2]
    
    # data file for testing modules
    f = codecs.open("test_data.csv", 'r', encoding = 'utf-8')
    numlines = 0
    dataD = {}
    for l in f:
        if numlines == 0:
            hdrL = l.strip().split(',')
        else:
            dataL = l.strip().split(',')
            dataD[int(dataL[0])] = map(lambda x: float(x), dataL[1:])
        numlines += 1
    f.close()
    
    idL = dataD.keys()
    idL.sort()
    wL = []
    xL = []
    yL = []
    for i in idL:
        wL.append(dataD[i][0])
        xL.append(dataD[i][1])
        yL.append(dataD[i][2])
        
    g = codecs.open("test_stats.csv", 'r', encoding = 'utf-8')
    numlines = 0
    for l in g:
        if numlines == 0:
            stats_hdrL = l.strip().split(',')
        else:
            statsL = map(lambda x: float(x), l.strip().split(','))
        numlines += 1
    g.close()
    
    count = int(statsL[0])

    print "4 tests for length of data"
    assertEqual(len(dataD.keys()), count)
    assertEqual(len(wL), count)
    assertEqual(len(xL), count)
    assertEqual(len(yL), count)

    # test for mean()
    print "\n2 tests each for mean and weighted mean"
    assertEqual(round(abs(1-mean(xL)/statsL[1]), 3), 0)
    assertEqual(round(abs(1-mean(yL)/statsL[2]), 3), 0)
    
    # test for wted mean()
    assertEqual(round(abs(1-mean(xL, wL)/statsL[3]), 3), 0)
    assertEqual(round(abs(1-mean(yL, wL)/statsL[4]), 3), 0)

    # test for cov_p(), weighted
    print "\n2 forms of weighted covariance"
    assertEqual(round(abs(1-cov_p(xL, yL, wL)/statsL[5]), 3), 0)
    assertEqual(round(abs(1-cov_p(x1L=xL, x2L=yL, wtL=wL)/statsL[5]), 3), 0)
        
    # test for cov_p(), unweighted
    print "\n3 forms of unweighted covariance"
    assertEqual(round(abs(1-cov_p(xL, yL)/statsL[6]), 3), 0)
    assertEqual(round(abs(1-cov_p(xL, yL, None)/statsL[6]), 3), 0)
    assertEqual(round(abs(1-cov_p(x1L=xL, x2L=yL)/statsL[6]), 3), 0)

    # test for variance_p(), weighted
    print "\n3 forms of weighted variance, + 1 extra test"
    assertEqual(round(abs(1-cov_p(xL, xL, wL)/statsL[7]), 3), 0)
    assertEqual(round(abs(1-cov_p(xL, None, wL)/statsL[7]), 3), 0)
    assertEqual(round(abs(1-cov_p(x1L =xL, wtL=wL)/statsL[7]), 3), 0)
    assertEqual(round(abs(1-cov_p(yL, yL, wL)/statsL[8]), 3), 0)

    # test for variance_p(), unweighted
    print "\n2 forms of unweighted variance, + 1 extra test"
    assertEqual(round(abs(1-cov_p(xL)/statsL[9]), 3), 0)
    assertEqual(round(abs(1-cov_p(xL, xL)/statsL[9]), 3), 0)
    assertEqual(round(abs(1-cov_p(yL)/statsL[10]), 3), 0)


if __name__ == '__main__':
  main()

#!/usr/bin/python
""" linear_fit.py
    test data in test_data.csv
    descriptive stat.s for testing in test_params.csv
"""
import os, sys, pickle, codecs
from stats_modules import *
from compare_vectors import *
from math import asin, sqrt, acos

# modules go here
def assertEqual(a,b):
    if a == b:
        print "Pass"
    else :
        print "Fail"

def fit_first_order(xL, yL, wtL=None):
    """Linear fit to a line
       Usage is positional with optional use of None for third argument (weights)
       fit_first_order(<xL>, <yL>, [<wL>|None])
    """
    if type(xL) == type([1]):
        n = len(xL)
    else:
        print "1st argument to fit_first_order() not list"
        sys.exit(2)
    if type(yL) == type([1]):
        if len(yL) == n:
            pass
        else:
            print "1st and 2nd arguments to fit_first_order() not equal length" 
    else:
        print "2nd argument to fit_first_order() not list"
        sys.exit(2)
    if wtL == None:
        wtL = [1 for i in range(n)]
    elif type(wtL) == type([1]):
        if len(wtL) == n:
            pass
        else:
            print "Optional third argument weights list not same length as data"
    else:
        print "weights argument to fit_first_order() not list"
        sys.exit(2)
    coeff1 = cov_p(xL,yL,wtL)/cov_p(xL,xL,wtL)
    coeff0 = mean(yL,wtL)-coeff1*mean(xL,wtL)
    residualVec = [yL[i] - (coeff1*xL[i] + coeff0) for i in range(n)]
    rmse = mean([err**2 for err in residualVec], wtL)**0.5
    return coeff1, coeff0, rmse, residualVec
    
def fit_two_variables(x0L, x1L, yL, wtL=None):
    """Linear fit to a plane
       Usage is positional with optional use of None for fourth argument (weights)
       fit_two_variables(<x0L>, <x1L>, <yL>, [<wL>|None])
    """
    if type(x0L) == type([1]):
        n = len(x0L)
    else:
        print "1st argument to fit_two_variables() not list"
        sys.exit(2)
    if type(x1L) == type([1]):
        if len(x1L) == n:
            pass
        else:
            print "1st and 2nd arguments to fit_two_variables() not equal length" 
    else:
        print "2nd argument to fit_two_variables() not list"
        sys.exit(2)
    if type(yL) == type([1]):
        if len(yL) == n:
            pass
        else:
            print "1st and 3rd arguments to fit_two_variables() not equal length" 
    else:
        print "3rd argument to fit_two_variables() not list"
        sys.exit(2)
    if wtL == None:
        wtL = [1 for i in range(n)]
    elif type(wtL) == type([1]):
        if len(wtL) == n:
            pass
        else:
            print "Optional fourth argument weights list in fit_two_variables() not same length as data"
    else:
        print "weights argument to fit_two_variables() not list"
        sys.exit(2)
    XL = [x0L, x1L]
    covarM = []
    for i in range(len(XL)):
        covarM.append([])
        for j in range(len(XL)):
            if j>=i:
                covarM[i].append(cov_p(XL[i],XL[j],wtL))
            else:
                covarM[i].append(covarM[j][i])
    det = covarM[0][0]*covarM[1][1] - covarM[0][1]**2
    covardata = [cov_p(XL[0],yL,wtL), cov_p(XL[1],yL,wtL)]
    inv_covarM = [[covarM[1][1]/det, -covarM[0][1]/det], [-covarM[1][0]/det, covarM[0][0]/det]]
    coeffL = [0, inv_covarM[0][0]*covardata[0] + inv_covarM[1][0]*covardata[1],
    inv_covarM[0][1]*covardata[0] + inv_covarM[1][1]*covardata[1]]
    coeffL[0] = mean(yL, wtL) - coeffL[1]*mean(XL[0], wtL) - coeffL[2]*mean(XL[1], wtL)
    residualVec = [yL[i] - (coeffL[1]*XL[0][i] + coeffL[2]*XL[1][i] + coeffL[0]) for i in range(n)]
    rmse = mean([err**2 for err in residualVec], wtL)**0.5
    return coeffL[2], coeffL[1], coeffL[0], rmse, residualVec

def fit_2nd_order(xL, yL, wtL=None):
    """Linear fit to a quadratic
       Usage is positional with optional use of None for third argument (weights)
       fit_2nd_order(<xL>, <yL>, [<wL>|None])
    """
    if type(xL) == type([1]):
        n = len(xL)
    else:
        print "1st argument to fit_2nd_order() not list"
        sys.exit(2)
    if type(yL) == type([1]):
        if len(yL) == n:
            pass
        else:
            print "1st and 2nd arguments to fit_2nd_order() not equal length" 
    else:
        print "2nd argument to fit_2nd_order() not list"
        sys.exit(2)
    if wtL == None:
        wtL = [1 for i in range(n)]
    elif type(wtL) == type([1]):
        if len(wtL) == n:
            pass
        else:
            print "Optional third argument weights list in fit_2nd_order() not same length as data"
    else:
        print "weights argument to fit_2nd_order() not list"
        sys.exit(2)
    xsqL = [x**2 for x in xL]
    return fit_two_variables(xL, xsqL, yL, wtL)

def main():
    if len(sys.argv) != 1: 
        print "usage $ linear_fit.py "
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
    resL = []
    for i in idL:
        wL.append(dataD[i][0])
        xL.append(dataD[i][1])
        yL.append(dataD[i][2])
        resL.append(dataD[i][3])
        
    g = codecs.open("test_params.csv", 'r', encoding = 'utf-8')
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

    # test for unweighted linear fit to a line fit_first_order()
    print "\ncoeff1, coeff0, rmse and residualVec for unweighted fit to a line"
    coeff1, coeff0, rmse, residualVec = fit_first_order(xL,yL)
    assertEqual(round(abs(1-coeff1/statsL[1]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[2]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[3]), 3), 0)
        
    # test for weighted linear fit to a line fit_first_order()
    print "\ncoeff1, coeff0, rmse and residualVec for weighted fit to a line"
    coeff1, coeff0, rmse, residualVec = fit_first_order(xL,yL,wL)
    assertEqual(round(abs(1-coeff1/statsL[4]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[5]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[6]), 3), 0)
            
    # test for unweighted linear fit to a plane fit_two_variables()
    xsqL = [x**2 for x in xL]
    print "\ncoeff2, coeff1, coeff0 and rmse for unweighted fit to a plane"
    coeff2, coeff1, coeff0, rmse, residualVec = fit_two_variables(xL, xsqL, yL)
    assertEqual(round(abs(1-coeff2/statsL[7]), 3), 0)
    assertEqual(round(abs(1-coeff1/statsL[8]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[9]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[10]), 3), 0)
        
    # test for unweighted linear fit to a quadratic fit_2nd_order()
    print "\ncoeff2, coeff1, coeff0 and rmse for unweighted fit to a quadratic"
    coeff2, coeff1, coeff0, rmse, residualVec = fit_2nd_order(xL, yL)
    assertEqual(round(abs(1-coeff2/statsL[7]), 3), 0)
    assertEqual(round(abs(1-coeff1/statsL[8]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[9]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[10]), 3), 0)
        
    # test for weighted linear fit to a plane fit_two_variables()
    print "\ncoeff2, coeff1, coeff0, rmse and residualVec (norm and angle) for weighted fit to a line"
    coeff2, coeff1, coeff0, rmse, residualVec = fit_two_variables(xL,xsqL,yL,wL)
    assertEqual(round(abs(1-coeff2/statsL[11]), 3), 0)
    assertEqual(round(abs(1-coeff1/statsL[12]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[13]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[14]), 3), 0)
    assertEqual(round(norm(residualVec, wL) - norm(resL,wL), 5), 0)
    assertEqual(round(angle(residualVec, resL, wL), 4), 0)

    # test for weighted linear fit to a quadratic fit_2nd_order()
    print "\ncoeff2, coeff1, coeff0, rmse and residualVec (norm and angle) for weighted fit to a quadratic"
    coeff2, coeff1, coeff0, rmse, residualVec = fit_2nd_order(xL,yL,wL)
    assertEqual(round(abs(1-coeff2/statsL[11]), 3), 0)
    assertEqual(round(abs(1-coeff1/statsL[12]), 3), 0)
    assertEqual(round(abs(1-coeff0/statsL[13]), 3), 0)
    assertEqual(round(abs(1-rmse/statsL[14]), 3), 0)
    assertEqual(round(norm(residualVec, wL) - norm(resL,wL), 5), 0)
    assertEqual(round(angle(residualVec, resL, wL), 4), 0)

if __name__ == '__main__':
  main()

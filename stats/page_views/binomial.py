#!/usr/bin/python                                                               
""" binomial.py"""

from math import log, e, pi, factorial, sqrt
from os import sys

def assertEqual(a,b=0):
    if a == b:
        print "Pass"
    else :
        print "Fail", str(a), '!=', str(b)

def product(lst):
    if type(lst) != type([1,2]) and type(lst) != type(set([1,2])):
        print "Argument to product has to be a list or set, exiting"
        sys.exit(2)
    else:
        for l in lst:
            if type(l) == type(2.) or type(l) == type(2):
                pass
            else:
                print "Elements of iterable argument to product() have to have * defined, exiting"
                sys.exit(2)
    prod = 1.
    for l in lst:
        prod *= l
    return prod
    
def binomial_exact(n, m):
    """ For integer arguments only. Returns the float valued
    binomial coefficient (n,m) = ^nC_m = (n)!/((n-m)! * m!),
    overflows at (1025,513), value 10^306 = 2^1023"""
    if type(n) != type(2) or type(m) != type(2) or n < 0 or m < 0 or n < m:
        print "Both arguments to binomial_exact have to be whole numbers, first argument >= second. Exiting."
        sys.exit(2)
    else:
        pass
    m = min(m, n-m) # imposes symmetry
    # Implements ^nC_m = prod_(i=1)^m (n-m+i)/i 
    return product([1.*(n-m+i)/i for i in range(1,m+1)])

def binomial_recursive(n, m):
    """ For integer arguments only. Returns the integer valued
    binomial coefficient (n,m) = ^nC_m = (n)!/((n-m)! * m!),
    overflows at (1025,513), value 10^306 = 2^1023"""
    if type(n) != type(2) or type(m) != type(2) or n < 0 or m < 0 or n < m:
        print "Both arguments to binomial_recursive have to be whole numbers, first argument >= second. Exiting."
        sys.exit(2)
    else:
        pass
    def bin_rec(p,q):
        if q==0:
            return 1
        else:
            return (1.*(p-q+1)/q)*bin_rec(p,q-1)
    # Uses the induction property on m for recursive definition
    # ^nC_m = ((n-m+1)/m)* ^nC_(m-1)
    # Can inductively prove Induction property on n
    # ^nC_m = (n/(n-m)) * ^(n-1)C_m
    m = min(m, n-m) # imposes symmetry
    return int(round(bin_rec(n,m),0))

def ln_binomial(n,m):
    """ returns the ln of binomial coefficient (n,m) = ^nC_m = (n)!/((n-m)! * m!)
    On (1024,512), the ln differs from exact by e-13
    , takes 10 s for (10^8,0.5*10^8), no overflow"""
    if type(n) != type(2) or type(m) != type(2) or n < 0 or m < 0 or n < m:
        print "Both arguments to ln_binomial have to be whole numbers, first argument >= second. Exiting"
        sys.exit(2)
    else:
        pass
    m = min(m, n-m) # imposes symmetry
    # Implements ln(^nC_m) = sum_(i=1)^m ln((n-m+i)/i) 
    lzt = [log(1.*(n-m+i)/i) for i in range(1,m+1)]
    return sum(lzt)

def ln_binomial_recursive(n, m):
    """ returns the ln of binomial coefficient (n,m) = ^nC_m = (n)!/((n-m)! * m!)
    On (1024,512), the ln differs from exact by e-13
    , takes 10 s for (10^8,0.5*10^8), no overflow"""
    if type(n) != type(2) or type(m) != type(2) or n < 0 or m < 0 or n < m:
        print "Both arguments to binomial_exact have to be whole numbers, first argument >= second. Exiting."
        sys.exit(2)
    else:
        pass
    def ln_bin_rec(p,q):
        if q==0:
            return 0
        else:
            return log((p-q+1)/q) + ln_bin_rec(p,q-1)
    # Uses the induction property on m for recursive definition
    # ln(^nC_m) = ln((n-m+1)/m) + ln(^nC_(m-1))
    # Can inductively prove Induction property on n
    # ln(^nC_m) = ln(n/(n-m)) + ln(^(n-1)C_m)
    m = min(m, n-m) # imposes symmetry
    return ln_bin_rec(n,m)

def ln_binomial_stirling(n,m):
    """ returns the ln of binomial coefficient (n,m) = ^nC_m = (n)!/((n-m)! * m!),
    using Stirling's Approx. for !, which is good to 10^-4 above 150. But we can simply use 1000 as a cut off
    - if n < 1000: ln(binomial_exact)
    - elif m < 500: Stirling's approximation for n! and (n-m)!
    - else: Stirling's approx for all three factorials n!, m!, (n-m)!
    On (1024,512), the ln differs from exact by 0.00025, e^log is off by 1/4000
    , no humanly measurable time for (10^12,0.5*10^12), no overflow"""
    if type(n) != type(2) or type(m) != type(2) or n < 0 or m < 0 or n < m:
        print "Both arguments to ln_binomial_stirling have to be whole numbers, first argument >= second. Exiting"
        sys.exit(2)
    else:
        pass
    m = min(m, n-m) # imposes symmetry
    if n<1000:
        return log(1.*binomial_exact(n,m))
    elif m >= 500:
        return log(sqrt(n/(2 * pi * m * (n-m)))) + m*log(n/m) + (n-m)*log(n/(n-m))
    else : # m < 500
        return (n-m-0.5)*log(n/(n-m)) + m*(log(n)-1) - log(factorial(m))
    
def main():
    
    # Test function product
    print "\n2 tests for product() on set and list, with float and negative"
    assertEqual(product(set([13,19,17])), 4199)
    assertEqual(product([-13,17.,19]), -4199.)

    """Tests where necessary properties of Binomial:
    ^nC_0 = 1
    Symmetry: ^nC_m = ^nC_(n-m)
    Induction property on m: ^nC_m = ((n-m+1)/m)* ^nC_(m-1)
    Induction property on n: ^nC_m = (n/(n-m)) * ^(n-1)C_m    """
    
    # Test function binomial_exact
    # Symmetry not tested
    print "\n2 tests for binomial_exact() on small integers"
    assertEqual(binomial_exact(150,0), 1)
    assertEqual(binomial_exact(5,2),10)

    print "\n2 tests for induction of binomial_exact() for n,m = 5, 2"
    n,m = 5,2
    assertEqual((n-m) * binomial_exact(n,m), n * binomial_exact(n-1, m))
    assertEqual(m * binomial_exact(n,m), (n-m+1) * binomial_exact(n, m-1))
  
    print "\n2 tests for induction of binomial_exact() for n,m = 999, 666"
    n,m = 999, 666
    assertEqual((n-m) * binomial_exact(n,m), n * binomial_exact(n-1, m))
    assertEqual((m * binomial_exact(n,m)) /((n-m+1)*binomial_exact(n, m-1))-1.)
  
    # Test function ln_binomial
    # Symmetry not tested
    print "\n2Tests for value of ln_binomial(), for small and large."
    assertEqual(int(round(e**(ln_binomial(10,5))/binomial_exact(10,5) - 1., 4)))
    assertEqual(int(round(e**(ln_binomial(1024,500))/binomial_exact(1024, 500) - 1., 4)))
    print "\n2 tests for induction of ln_binomial() for n,m = 999, 666"
    n,m = 999, 666
    assertEqual(int(round((1.*(n-m)/n) * e**(ln_binomial(n,m) - ln_binomial(n-1, m))-1., 4)))
    assertEqual(int(round((1.*m/(n-m+1)) * e**(ln_binomial(n,m) - ln_binomial(n, m-1))-1., 4)))

    # Test function ln_binomial_stirling
    print "\n2Tests for values of ln_binomial_stirling(), large, larger and huge"
    assertEqual(int(round(e**(ln_binomial_stirling(999, 333))/binomial_exact(999, 333)-1., 4)))
    assertEqual(ln_binomial_stirling(2000, 450)/ln_binomial(2000, 450)-1)
    assertEqual(int(round(e**(ln_binomial_stirling(100000000, 50000000)-ln_binomial(100000000, 50000000))-1., 4)))
    print "\n2 tests for induction of ln_binomial_stirling() for exact, n,m = 999, 666"
    n,m = 999, 666
    assertEqual(int(round((1.*(n-m)/n) * e**(ln_binomial_stirling(n,m) - ln_binomial_stirling(n-1, m))-1., 4)))
    assertEqual(int(round((1.*m/(n-m+1)) * e**(ln_binomial_stirling(n,m) - ln_binomial_stirling(n, m-1))-1., 4)))
    print "\n2 tests for induction of ln_binomial_stirling() with m! for n,m = 2000, 450"
    n,m = 2000, 450
    assertEqual(int(round((1.*(n-m)/n) * e**(ln_binomial_stirling(n,m) - ln_binomial_stirling(n-1, m))-1., 4)))
    assertEqual(int(round((1.*m/(n-m+1)) * e**(ln_binomial_stirling(n,m) - ln_binomial_stirling(n, m-1))-1., 4)))
    print "\n2 tests for induction of ln_binomial_stirling() for n,m = 1000000, 500000"
    n,m = 1000000, 500000
    assertEqual(int(round(log(1.*(n-m)/n) + ln_binomial_stirling(n,m) - ln_binomial_stirling(n-1, m), 4)))
    assertEqual(int(round((1.*m/(n-m+1)) * e**(ln_binomial_stirling(n,m) - ln_binomial_stirling(n, m-1))-1., 4)))


if __name__ == '__main__':
  main()


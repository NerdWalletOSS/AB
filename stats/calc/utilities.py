#!/usr/bin/python                                                               
""" utilities.py
    write two-column csv
    R script for csv2jpeg
    exec R script
    make jpeg by calling above three
    show jpeg
    makes an array of quantiles
    calculates first difference of iterable
"""

import numpy as np
import os
import pandas as pd
import sys

from subprocess import call


def write_csv(xL, yL, f_name, x_hdr = 'x', y_hdr = 'y'):
    """ Writes two column data to file with headers
        
    Arg.s:
        list of x data
        list of y data
        file name
        x header
        y header

    Returns:
        f_name.csv with 
        'x_hdr', 'y_hdr'
        xL[0], yL[0]
        ...
    """
    if len(xL) != len(yL):
        print "unequal length lists, exiting"
        sys.exit(1)
    f_full_name = "_" + f_name + ".csv"
    f_path = os.path.realpath(os.path.join(os.getcwd(), f_full_name))
    dat = np.transpose([xL, yL])
    df = pd.DataFrame(columns = [x_hdr, y_hdr], data = dat)
    df.to_csv(f_path, index = False)
    print "\nWrote csv", f_full_name

def gen_r_script_csv2jpeg(f_name):
    """ Example
    Generate R script to:
    Read two-column csv data 
    Make a simple xy plot in jpeg
    Does NOT execute .R!

    Arg.s:
        base file name

    Returns:
        f_name.R
    """
    r_script_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".R"))
    csv_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".csv"))
    jpeg_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".jpeg"))

    f = open(csv_name, 'r')
    headL = f.readline().strip().split(',')
    f.close()
    x_hdr = headL[0].strip()
    y_hdr = headL[1].strip()

    r = open(r_script_name, 'w')
    r.write('xydata <- read.csv("'+csv_name+'")')
    r.write("\nattach(xydata)")
    r.write('\njpeg(filename = "'+jpeg_name+'")')
    r.write("\nplot("+x_hdr+", "+y_hdr+", type = 'b', main = '"+f_name+"')")
    r.write("\ndev.off()")
    r.close()

def exec_r_script(f_name):
    r_script_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".R"))
    r_cmd = "R CMD BATCH " + r_script_name
    call(r_cmd.split(" "))

def make_jpeg(xL, yL, f_name="xydata", xhdr="x", yhdr="y"):
    """ Makes a .jpeg from two column data with headers

    Arg.s:
        x data
        y data
        base file name
        x header string (optional)
        y header string (optional)

    Returns:
        jpeg file
    """
    write_csv(xL, yL, f_name, xhdr, yhdr)
    gen_r_script_csv2jpeg(f_name)
    exec_r_script(f_name)
    print "Made jpeg", "_"+f_name+".jpeg"
   
def show_jpeg(f_name):
    jpeg_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".jpeg"))
    show = "xdg-open " + jpeg_name
    # Tried "eog " + jpegname + " &", worked once
    # then stopped working, xdg-open redirects to default viewer, which is eog!
    # seems to work!
    call(show.split(" "))

def make_quantile_array(volume, length):
    """ Make a quantile Array of length "length" to symetrically span 
    "volume" % of the distribution

    Arg.s:
        volume of distribution to be covered
        length of array

    Returns: an array of length "length"
        the difference between whose terminal elements is = "volume"
    """
    if not(0. < volume < 1.):
        print "!(0 < volume < 1), exiting"
        sys.exit(1)
    if isinstance(length, int):
        if length < 2:
            print "length is an integer < 2, exiting"
            sys.exit(1)
    else:
        print "length not an integer, exiting"
        sys.exit(1)
    pmin = 1.0 * (1 - volume) / 2
    step = 1.0 * volume / (length - 1)
    ppL = [pmin + i*step for i in range(length)] # quantiles 
    return np.array(ppL)

def first_diff(iterable1d):
    """ Takes the first difference of the elements of a 1D array or list,
    averaged over a rolling window of length 3
    assuming it is padded by the first and last elements

    Args:
        single 1D array or a list with datatype with "-" defined
        
    Returns:
        same type and length as original argument
    """
    if not(isinstance(iterable1d, (np.ndarray, list))):
        print "argument not list or 1D array, exiting"
        sys.exit(1)
    l = len(iterable1d)
    diffL = [(iterable1d[min(i + 1, l - 1)] - iterable1d[max(i - 1, 0)]) / 2.
             for i in range(l)]
    if isinstance(iterable1d, np.ndarray):
        return np.array(diffL)
    else:
        return diffL


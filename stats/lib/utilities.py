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

import matplotlib as mpl
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import sys

from subprocess import call
from math import log10, floor

def sig_digits(x, n=2):
    if x == 0 or x == 0.:
        return 0
    else:
        return round(x, -int(floor(log10(abs(x)))) + (n - 1))


def check_file(file, action = "none", supp = 0):
    if os.path.exists(file):
        if supp == 0:
            pass
        else:
            print action, file
    else:
        print '\nNOT', action, file, ", exiting"
        sys.exit(0)
    

def save(path, ext='jpeg', close=True, verbose=False):
    """ Save a figure from pyplot.
        From jesshamrick.com/2012/09/03/saving-figures-from-pyplot/
    Parameters
    ----------
    path : string
        The path (and filename, without the extension) to save the
        figure to.
    ext : string (default='jpeg')
        The file extension. This must be supported by the active
        matplotlib backend (see matplotlib.backends module).  Most
        backends support 'png', 'pdf', 'ps', 'eps', and 'svg'.
    close : boolean (default=True)
        Whether to close the figure after saving.  If you want to save
        the figure multiple times (e.g., to multiple formats), you
        should NOT close it in between saves or you will have to
        re-plot it.
    verbose : boolean (default=True)
        Whether to print information about when and where the image
        has been saved.
    """
    
    # Extract the directory and filename from the given path
    directory = os.path.split(path)[0]
    filename = "%s.%s" % (os.path.split(path)[1], ext)
    if directory == '':
        directory = '.'

    # If the directory does not exist, create it
    if not os.path.exists(directory):
        os.makedirs(directory)

    # The final path to save to
    savepath = os.path.join(directory, filename)

    if verbose:
        print("Saving figure to '%s'..." % savepath),

    # Actually save the figure
    plt.savefig(savepath)
    
    # Close it
    if close:
        plt.close()

    if verbose:
        print("Done")


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
    f_full_name = f_name + ".csv"
    # f_path = os.path.realpath(os.path.join(os.getcwd(), f_full_name))
    f_path = f_full_name
    dat = np.transpose([xL, yL])
    df = pd.DataFrame(columns = [x_hdr, y_hdr], data = dat)
    df.to_csv(f_path, index = False)
    check_file(f_full_name, "written")

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

def make_r_jpeg(xL, yL, f_name="xydata", xhdr="x", yhdr="y"):
    """ Uses R, makes a .jpeg from two column data with headers

    Arg.s:
        x data
        y data
        base file name
        x header string (optional)
        y header string (optional)

    Returns:
        jpeg file
    """
    # Error msgs?
    write_csv(xL, yL, f_name, xhdr, yhdr)
    gen_r_script_csv2jpeg(f_name)
    exec_r_script(f_name)
    print "Made R jpeg", "_"+f_name+".jpeg"
   
def makeJpegNew(xL, yL, outName, xhdr, yhdr, pltlbl = "plot_lbl"):
    f_name = outName
    # write_csv(xL, yL, f_name, xhdr, yhdr)
    mpl.rcParams['xtick.direction'] = 'out'
    mpl.rcParams['ytick.direction'] = 'out'
    plt.figure(figsize = (10,8))
    # plt.axes().set_aspect('equal')
    plt.tick_params(labelsize = 20)
    plt.plot(xL, yL, 'k-', label = pltlbl)
    plt.title(yhdr + " vs. " + xhdr, fontsize = 28)
    plt.xlabel(xhdr, fontsize = 22)
    plt.ylabel(yhdr, fontsize = 22)
    plt.legend(loc = 'lower center', fontsize = 22)
    save(f_name)
    #check_file(f_name + ".jpeg", "made")


def make_jpeg(xL, yL, outName="./Output/xy_", xhdr="x", yhdr="y", pltlbl = "plot_lbl"):
    """ Makes a .jpeg from two column data with headers

    Arg.s:
        x data
        y data
        path to base file name
        x header string (optional)
        y header string (optional)

    Returns:
        jpeg file
    """
    # Error msgs.s?
    f_name = outName + yhdr + '_vs_' + xhdr
    # write_csv(xL, yL, f_name, xhdr, yhdr)
    mpl.rcParams['xtick.direction'] = 'out'
    mpl.rcParams['ytick.direction'] = 'out'
    plt.figure(figsize = (10,8))
    # plt.axes().set_aspect('equal')
    plt.tick_params(labelsize = 20)
    plt.plot(xL, yL, 'k-', label = pltlbl)
    plt.title(yhdr + " vs. " + xhdr, fontsize = 28)
    plt.xlabel(xhdr, fontsize = 22)
    plt.ylabel(yhdr, fontsize = 22)
    plt.legend(loc = 'lower center', fontsize = 22)
    save(f_name)
    check_file(f_name+".jpeg", "made")
   
def show_jpeg(jpeg_name):
    # Error msgs?
    # jpeg_name = os.path.realpath(os.path.join(os.getcwd(), "_"+f_name+".jpeg"))
    if jpeg_name.split('.')[-1] == 'jpeg':
        pass
    else:
        jpeg_name += '.jpeg'
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
    pmin = 0.5 - volume/2.
    step = volume / (length - 1.)
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


def make_tex_table(tablename, hdrS, dataL):
    ncol = len(hdrS.split('&'))
    f = open(tablename + '.tex', 'w')
    f.write("\centering\n")
    f.write('\\begin{tabular}{|' + 'r|'*ncol + '} \hline \hline\n')
    hdrS += '\\\\ \hline\n'
    f.write(hdrS)
    for rowS in dataL:
        if len(rowS.split('&')) == ncol:
            pass
        else:
            print "length of ", rowS, "not same as num(col.) =", ncol, "exiting"
            sys.exit(0)
        rowS += ' \\\\ \hline\n'
        f.write(rowS)
    f.write('\hline\n')
    f.write('\end{tabular}\n')
    f.close()


def input_tex_table(to_fileF, tablename, caption = "caption", label = "table:name"):
    f = open(to_fileF, 'a')
    f.write("\\begin{table}[ht!]\n")
    f.write("\input{"+tablename+".tex}\n")
    f.write("\caption{"+caption+"}\n")
    f.write("\label{"+label+"}\n")
    f.write("\end{table}\n\n")
    f.close()


def input_tex_figure(to_fileF, figname, caption = "Figure", label = "fig:name"):
    f = open(to_fileF, 'a')
    f.write("\\begin{figure}[ht!]\n")
    f.write("\centering\n")
    f.write("\includegraphics[width=90mm]{"+figname+"}\n")
    f.write("\caption{"+caption+"}\n")
    f.write("\label{"+label+"}\n")
    f.write("\end{figure}\n\n")
    f.close()

#!/usr/bin/python                                                               
"PageViewsStats.py"

import json
import numpy as np
import os
import pandas as pd
import sys

from math import erf, sqrt, log10
from subprocess import call

sys.path.insert(0, os.path.abspath('..'))

from lib import utilities as u
from lib import ab_stats_functions as fn
from lib import stats_modules as sm


def Phi(z):
    """ parametrized credibility,
        integral of a normalized normal distribution
        1- Phi = p-value """
    return (1.+erf(z/sqrt(2.)))/2.


def product(lst): # product of the elements of a list, do numpy array later"
    if type(lst) != type([1,2]) and type(lst) != type(set([1,2])):
        print "Argument to product has to be a list or set, exiting"
        sys.exit(2)
    else:
        for l in lst:
            if type(l) == type(2.) or type(l) == type(2):
                pass
            else:
                print "Elements of iterable argument to product()\
                have to have * defined, exiting"
                sys.exit(2)
    prod = 1.
    for l in lst:
        prod *= l
    return prod
    

# unclear why we need this
def PhiCorr(p, mean_p, sigma_p):
    """ For distributions on probability space = [0,1],
        this corrects for the finite value of Phi()
        at the end points of the unit interval."""
    return ((Phi((p-mean_p)/sigma_p) - Phi((-mean_p)/sigma_p))/
                (Phi((1.-mean_p)/sigma_p) - Phi((-mean_p)/sigma_p)) )


def main():
    f_path = "/Users/ranjeettate/Documents/WORK/AB/stats/PageViews/"
    if len(sys.argv) == 1:
        print "Usage: $python PageViewsStats.py test_name page_views_lift"
        print "Data expected in ./Data/[test_name].txt"
        print "page_views_lift > 0"
        print "using defaults FakeTest 0.15"
        test_name = "FakeTest" # may include ramp number
        page_views_lift = 0.15
    elif len(sys.argv) == 3:
        test_name = sys.argv[1]
        try:
            page_views_lift = float(sys.argv[2])
        except ValueError:
            print "Second Argument not floatable, exiting"
            sys.exit(0)
    else:
        print "Wrong number of arguments, exiting"
        sys.exit(0)

    # remove output files and (re)make directory
    outS = "./Output/" + test_name + '/'
    rmS = "rm -fr " + outS
    mkS = "mkdir " + outS
    call(rmS.split(' '))
    call(mkS.split(' '))
    u.check_file(outS, "(re)made dir")

    tex_templateF = "results_template.tex"
    tex_resultsF = outS + "results.tex"
    t = open(tex_templateF, 'r')
    r = open(tex_resultsF, 'w')
    tex_test_name = test_name.replace('_', '')
    for line in t:
        newline = line.replace('$testname', tex_test_name)
        r.write(newline)
    t.close()
    r.close()

    tex_analysisF = outS + "analysis.tex"
    a = open(tex_analysisF, 'w')
    a.write("\n\section{"+tex_test_name + "}\n")
    a.write("\subsection{Data}\n")
    a.write("Blah Blah the original data.\n")
    a.close()
    
    # read data and create data dictionary {variant: {userid: page_views}}
    fname = "./Data/" + test_name + ".txt"
    variantUserPagesD = {}
    lnum = 0
    uuidL = []
    Zero_PV_uuidL = []
    u.check_file(fname, "Found data file")
    f = open(fname, 'r')
    splitchar = ','
    # Split on splitchar
    for line in f:
        if lnum == 0:
            print "Headers in line", str(lnum), ':', line
            headerL = line.strip().split(',')
            lnum += 1
        else:
            lL = line.strip().split(',')
            variant = lL[0]
            uuid = lL[1]
            if uuid in uuidL: # tested on FakeDataFail1/2 (Same or different variants).
                # Edit FailConfig
                print "Constraint 0 failed: non-unique uuid =", uuid, ", exiting"
                sys.exit(2)
            else:
                uuidL.append(uuid)
            try: # Tested on FakeDataFail3. Edit FailConfig
                page_views = int(lL[2])
            except ValueError:
                print "For variant, uuid =", variant, ',', uuid, \
                      "non-integer page_views =", lL[2], ", exiting"
                sys.exit(2)
            if variant in variantUserPagesD:
                pass
            else:
                variantUserPagesD[variant] = {}
            if page_views == 0:
                Zero_PV_uuidL.append(uuid)
            else:
                variantUserPagesD[variant][uuid] = page_views
            lnum += 1 
    f.close()
    #variantUserPagesD.keys()

    num_users_Zero_PV = len(Zero_PV_uuidL)
    
    gname = os.path.join(outS, "SummaryFreqStats.txt")
    g = open(gname, 'w')
    

    num_users_data = lnum-1
    print "Number of users in data =", num_users_data
    g.write(test_name + ', ' + start_date + ', ' + end_date)
    g.write("\n\nNumber of users in data = "+ str(num_users_data))
    print "Number of UUIDs with zero page_views =", num_users_Zero_PV 
    g.write("\nNumber of UUIDs with zero page_views ="+ str(num_users_Zero_PV)) 
    
    num_users_dict = 0
    for variant in variantUserPagesD:
        num_users_variant = len(variantUserPagesD[variant])
        print "Number of users in variant", variant, " =",\
            str(num_users_variant)
        num_users_dict += num_users_variant
    if num_users_dict == num_users_data - num_users_Zero_PV:
        pass
    else:
        print "Consistency fail: num_users in data + num_users(0PV) != variantUserPagesD, exiting"
        sys.exit(2)

    # subtract the first pageview from data
    for variant in variantUserPagesD:
        for user in variantUserPagesD[variant]:
            variantUserPagesD[variant][user] -= 1

    """# Write data files for each variant for R
    hdrS = ','.join(headerL[1:])
    for variant in variantUserPagesD:
        fname = "./Output/_" + test_name + "_" + variant +".csv"
        f = open(fname, 'w')
        f.write(hdrS)
        for user in variantUserPagesD[variant]:
            f.write('\n'+user + ',' + str(variantUserPagesD[variant][user]))
        f.close()
        u.check_file(fname, "For R, wrote: " + hdrS + " to:")"""
        
    # start write script.R
    Rfile_name = "PageViewsPlots.R"
    Rfull_file_name = os.path.join(outS, Rfile_name)
    RF = open(Rfull_file_name, 'w')
    RF.write('#!/usr/bin/env Rscript\n')
    # RF.write('initial.dir<-getwd()\n')
    # RF.write('setwd("./Output/'+test_name+'")\n')

    """u.check_file("R_plot_template.txt")
    for variant in variantUserPagesD:
        RTemplateF = open(fTname, "r")
        for line in RTemplateF:
            newline = line.replace("$Variant", variant)
            RF.write(newline)
        RF.write("\n")
        RTemplateF.close()"""


    # Calculate Frequentist descriptive stat.s for each variant
    variantL = variantUserPagesD.keys()
    variantL.sort()
    outD =  {}
    outD['Freq_metrics'] = {}
    outD['Freq_metrics']['desc'] = {}
    variantFreqStatsD = {}
    hdr1S = "Approach & Metric" 
    for variant in variantL:
        hdr1S += " & " + variant
        pagesL = variantUserPagesD[variant].values()
        num_users = len(variantUserPagesD[variant])
        min_pages = min(pagesL)
        max_pages = max(pagesL)
        mean_pages = sm.mean(pagesL)
        var_pages = sm.cov_p(pagesL)
        SD_pages = sqrt(var_pages)
        var_mean = var_pages/num_users
        SE_mean = sqrt(var_mean)
        variantFreqStatsD[variant] = [num_users, min_pages, max_pages, mean_pages, var_pages, var_mean, SE_mean]
        outD['Freq_metrics']['desc'][variant] = {'num users': num_users,\
                                        'min page views': min_pages,\
                                        'max page views': max_pages,\
                                        'mean page views': mean_pages,\
                                        'SE(mean page views)': SE_mean}
        # print Frequentist descriptive stat.s values
        print "\nVariant =", variant 
        print "number of users =", num_users
        print "Min. page views =", min_pages
        print "Max. page views =", max_pages
        print "Mean page views =", round(mean_pages, 3)
        print "Var(Page Views) =", round(var_pages, 3)
        print "SD(Page Views) =", round(SD_pages, 2)
        print "Var(mean_page_views) =", round(var_mean, 5)
        print "SE(mean_page_views) =", round(SE_mean, 3)
        g.write("\n\nVariant = " + variant )
        g.write("\nnumber of users = " + str(num_users))
        g.write("\nMin. page views = " + str(min_pages))
        g.write("\nMax. page views = " + str(max_pages))
        g.write("\nMean page views = " + str(round(mean_pages, 3)))
        g.write("\nVar(Page Views) = " + str(round(var_pages, 3)))
        g.write("\nSD(Page Views) = " + str(round(SD_pages, 2)))
        g.write("\nVar(mean_page_views) = " + str(round(var_mean, 5)))
        g.write("\nSE(mean_page_views) = " + str(round(SE_mean, 3)))

    g.close()
    u.check_file(gname, "Wrote summary Freq. stat.s to:")

    row1L = []
    fldL = ['num users', 'min page views', 'max page views',\
             'mean page views', 'SE(mean page views)']
    for fld in fldL:
        rowS = "Hist. & "+fld
        if fld in ('num users', 'min page views', 'max page views'):
            for var in variantL:
                rowS += " & " + str(int(u.sig_digits(outD['Freq_metrics']['desc'][var][fld], 3)))
        else:
            for var in variantL:
                rowS += " & " + str(u.sig_digits(outD['Freq_metrics']['desc'][var][fld], 3))
        row1L.append(rowS)

    print "\nSummary of Frequentist Stat.s"
    print "variant, mean_page_views, SE_mean_pv"
    for var in variantFreqStatsD:
        pv, SE_pv = variantFreqStatsD[var][3], variantFreqStatsD[var][6]
        print var, round(pv, 2), round(SE_pv, 4)
        
    nvar = len(variantL)

    # variant pairs in which mean(page_views): A >= B
    variantPairL = []
    for i in range(nvar-1):
        for j in range(i+1, nvar):
            varA, varB = variantL[i], variantL[j]
            pvA, pvB = variantFreqStatsD[varA][3], variantFreqStatsD[varB][3]
            if pvA >= pvB:
                variantPairL.append((varA, varB))
            else:
                variantPairL.append((varB, varA))

    # Frequentist comparison stats for each variant pair
    compairD = {}
    hdr2S = "Metric & Value & A & B & Cred.(A vs. B)"
    print "\nvariantA, variantB, lift%, z(lift), Freq. Conf."
    for varp in variantPairL:
        row2L = []
        rowlS = "Histo. PV Lift & "+str(page_views_lift*100) + '\%'
        row0S = "Histo. PV Lift & "+str(0) + '\%'
        varA, varB = varp
        pair = varA+'_'+varB
        compairD[pair] = {}
        compairD[pair]["hdr2S"] = hdr2S
        pvA, SE_pvA = variantFreqStatsD[varA][3], variantFreqStatsD[varA][6]
        pvB, SE_pvB = variantFreqStatsD[varB][3], variantFreqStatsD[varB][6]

        lf = page_views_lift
        zprop = (pvA - (1 + lf) * pvB)/sqrt(SE_pvA**2 + (1 + lf)**2 * SE_pvB**2)
        freq_conf = u.sig_digits(fn.conf(zprop), 3)
        print varA, varB, lf*100, u.sig_digits(zprop), round(freq_conf*100,1)
        rowlS += " & " + varA+' & '+varB + ' & ' + str(round(freq_conf*100,1))

        lf0 = 0
        zprop0 = (pvA - (1+lf0) * pvB)/sqrt(SE_pvA**2 + (1+lf0)**2 * SE_pvB**2)
        freq_conf0 = u.sig_digits(fn.conf(zprop0), 3)
        print varA, varB, lf*100, u.sig_digits(zprop0), round(freq_conf0*100,1)
        row0S += " & " + varA+' & '+varB + ' & ' + str(round(freq_conf0*100,1))

        row2L.append(rowlS)
        row2L.append(row0S)
        compairD[pair]["row2L"] = row2L

    LiftConfD = {}
    for varp in variantPairL:
        varA, varB = varp
        pvA, SE_pvA = variantFreqStatsD[varA][3], variantFreqStatsD[varA][6]
        pvB, SE_pvB = variantFreqStatsD[varB][3], variantFreqStatsD[varB][6]
        mean_lift = pvA/pvB - 1
        SE_mean_lift = sqrt((SE_pvA/pvB)**2 + (pvA*SE_pvB/pvB**2)**2)
        # percent increase in number of page views deemed important
        liftL = [mean_lift + x*(4*SE_mean_lift/20) - 2*SE_mean_lift \
                 for x in range(21)]
        LiftConfD[varp] = {}
        for lf in liftL:
            zlift = (pvA - (1+lf) * pvB)/sqrt(SE_pvA**2 + (1+lf)**2 * SE_pvB**2)
            freq_conf = u.sig_digits(fn.conf(zlift), 3)
            LiftConfD[varp][lf] = (zlift, freq_conf)

            
    # Write csv of lift, z, confidence level for each variant-pair for R
    hdrS = ','.join(['lift','z', 'confidence_level'])
    for varp in LiftConfD:
        varA, varB = varp
        fname = os.path.join(outS, varA +"_"+ varB +"_conf.csv")
        f = open(fname, 'w')
        f.write(hdrS)
        for lift in liftL:
            z = LiftConfD[varp][lift][0]
            conf = LiftConfD[varp][lift][1]
            f.write('\n'+str(lift) + ',' + str(z) + ',' + str(conf))
        f.close()
        print "\nWrote z, credibility as functions of delta to", fname 
        
    # write confidence plots to script.R
    u.check_file("R_conf_template.txt")
    for varp in LiftConfD:
        varA, varB = varp
        RTemplateF = open("R_conf_template.txt", "r")
        for line in RTemplateF:
            newline1 = line.replace("$var", var)
            newline = newline1.replace("$TestName", test_name)
            RBodyF.write(newline)
        RBodyF.write("\n")
        RTemplateF.close()




    RBodyF.close()

    fRHname = "R_hdr.txt"
    fRFname = "R_footer.txt"
    fSname = "_PageViewsPlots.R"
    sF = open(fSname, 'w')
    for fname in [fRHname, fRBname, fRFname]:
        f = open(fname, 'r')
        for line in f:
            sF.write(line)
        f.close()
    sF.close()
    
    # Call R to create png
    Rfile_name = "_PageViewsPlots.R"
    Rcmd = "R CMD BATCH " + Rfile_name
    Popen(Rcmd.split(' '), cwd = outS).wait()

    # check for files created by R
    """for variant in variantUserPagesD.keys():
        for ftype in ["_data.csv", "_hist.png", "_density.png"]:
            fname = os.path.join(outS, variant+ftype)
            u.check_file(fname, "R read csv or created png")"""

    for varp in variantPairL:
        varA, varB = varp
        for ftype in ["_conf.csv", "_conf.png", "_z.png"]:
            fname = os.path.join(outS, varA + '_' + varB + ftype)
            u.check_file(fname, "R read csv or created png")


    outD['Freq_metrics']['comp.'] ={}
    for varp in LiftConfD:
        varA, varB = varp
        pair = varA + "_" + varB + "_"
        for ftype in ["conf.csv", "z.png", "conf.png"]:
            fname = os.path.join(outS, pair+ftype)
            u.check_file(fname, "R read csv or created png")
            outD['Freq_metrics']['comp.'][pair[:-1]] = {ftype: fname} 
    

    print "\nR created various .png files,", "./Output/_"+test_name+"*.png\n"
    
    # Tate metrics

    #Create page views histogram dictionary
    print "\n******* Starting non-Frequentist analysis ********\n"

    variantPagesUsersD = {}
    for variant in variantUserPagesD:
        variantPagesUsersD[variant] = {}
        for user in variantUserPagesD[variant]:
            pages = variantUserPagesD[variant][user]
            if pages in variantPagesUsersD[variant]:
                variantPagesUsersD[variant][pages][0] += 1
            else:
                variantPagesUsersD[variant][pages] = [1]
    num_users_dict = 0
    for variant in variantPagesUsersD:
        for pages in variantPagesUsersD[variant]:
            num_users_dict += variantPagesUsersD[variant][pages][0]
    if num_users_dict + num_users_Zero_PV == num_users_data:
        pass
    else:
        print "Consistency fail: num_users in Dict + num_users(0PV) != \
in variantPagesUsersD, exiting"
        sys.exit(2)
    # print '\n', variantPagesUsersD

    # Hist of page views
    variantPagesHistD = {}
    for variant in variantPagesUsersD:
        variantPagesHistD[variant] = {}
        for pgs in variantPagesUsersD[variant]:
            users = variantPagesUsersD[variant][pgs][0]
            variantPagesHistD[variant][pgs] = users

    # fill in blank page numbers with 0 users
    for variant in variantPagesUsersD:
        maxpages = outD['Freq_metrics']['desc'][variant]['max page views']
        pagesL = range(maxpages)
        for pages in pagesL:
            if pages in variantPagesUsersD[variant]:
                pass
            else:
                variantPagesUsersD[variant][pages] = [0]
    # print '\n', variantPagesUsersD
        
    # cumulative sum = number of users that had that many page views
    for variant in variantPagesUsersD:
        mp = outD['Freq_metrics']['desc'][variant]['max page views']
        variantPagesUsersD[variant][mp].append(variantPagesUsersD[variant][mp][0])
        pages = mp - 1
        while pages >= 0:
            variantPagesUsersD[variant][pages].append(variantPagesUsersD[variant][pages][0] + variantPagesUsersD[variant][pages + 1][1])
            pages -= 1

    variantBinaryCountsD = {}
    for variant in variantPagesUsersD:
        trials = sum(map(lambda i: variantPagesUsersD[variant][i][1], variantPagesUsersD[variant].keys()))
        successes = trials - variantPagesUsersD[variant][1][1]
        variantBinaryCountsD[variant] = (trials, successes)

    print '\n'
    # log_10(num users) vs. pages visited 
    variantPagesLogUsersD = {}
    for variant in variantPagesUsersD:
        variantPagesLogUsersD[variant] = {}
        for pages in variantPagesUsersD[variant].keys():
            #print variantPagesUsersD[variant][pages]
            num_users = variantPagesUsersD[variant][pages][1]
            if num_users == 0:
                print "Error: Cum. users who've passed thru", pages, "pages in variant", variant, "=", num_users
            else:
                variantPagesLogUsersD[variant][pages] = log10(num_users)
    # print '\n', variantPagesLogUsersD

    # plot page view histogram
    xhdr = "pages"
    for variant in variantPagesHistD:
        outD['Tate_metrics']['figures'][variant] = {}
        yhdr = "numUsers"
        oname = outS + variant + '_'
        fname = oname + yhdr + '_vs_' + xhdr
        pgsL = variantPagesHistD[variant].keys()
        pgsL.sort()
        numUsersL = []
        for pgs in pgsL:
            numUsersL.append(variantPagesHistD[variant][pgs])
        u.make_jpeg(pgsL, numUsersL, oname, xhdr, yhdr, "Histogram_"+variant)
        u.show_jpeg(fname)
        outD['Tate_metrics']['figures'][variant]['pv_histo'] = fname

    # plot page views log histogram
    xhdr = "pages"
    for variant in variantPagesHistD:
        yhdr = "log10users"
        pgsL = variantPagesHistD[variant].keys()
        pgsL.sort()
        logUsersL = []
        for pgs in pgsL:
            logUsersL.append(log10(variantPagesHistD[variant][pgs]))
        oname = outS + variant + '_'
        fname = oname + yhdr + '_vs_' + xhdr
        u.make_jpeg(pgsL, logUsersL, oname, xhdr, yhdr, "LogHistogram_"+variant)
        u.show_jpeg(fname)
        outD['Tate_metrics']['figures'][variant]['pv_log_histo'] = fname

    # plot cumulative user counts
    xhdr = "min_pages"
    for variant in variantPagesUsersD:
        yhdr = "numUsers"
        oname = outS + variant + '_'
        fname = oname + yhdr + '_vs_' + xhdr
        pgsL = variantPagesUsersD[variant].keys()
        pgsL.sort()
        numUsersL = []
        for pgs in pgsL:
            numUsersL.append(variantPagesUsersD[variant][pgs][1])
        u.make_jpeg(pgsL, numUsersL, oname, xhdr, yhdr, variant)
        u.show_jpeg(fname)
        outD['Tate_metrics']['figures'][variant]['users_vs_min_pv'] = fname

    # plot log cumulative user counts
    xhdr = "min_pages"
    for variant in variantPagesLogUsersD:
        yhdr = "log10users"
        oname = test_name + '_' + variant
        fname = yhdr + '_vs_' + xhdr + '_' + oname
        pgsL = variantPagesLogUsersD[variant].keys()
        pgsL.sort()
        logUsersL = []
        for pgs in pgsL:
            logUsersL.append(variantPagesLogUsersD[variant][pgs])
        oname = outS + variant + '_'
        fname = oname + yhdr + '_vs_' + xhdr
        u.make_jpeg(pgsL, logUsersL, oname, xhdr, yhdr, variant)
        u.show_jpeg(fname)
        outD['Tate_metrics']['figures'][variant]['log10users_vs_min_pv'] = fname

    texFileL = []
    # ############ short time scale for Binary AB test
    outD['Tate_metrics']['short_time'] = {}
    outD['Tate_metrics']['short_time']['single'] = {}
    outD['Tate_metrics']['short_time']['comp.'] = {}
    variantBinaryCountsShortD = {}
    trialS = "Test Page & Trials"
    succS = "Test Page & Successes"
    inelasS = "Test Page & Inelasticity"
    SEinelasS = "Test Page & SE(Inelasticity)"
    for variant in variantL:
        trials = variantPagesUsersD[variant][1][1]
        successes = variantPagesUsersD[variant][2][1]
        inelas = fn.mu_freq(trials, successes)
        SEinelas = fn.var_mu_freq(trials, successes)
        variantBinaryCountsShortD[variant] = (trials, successes)
        outD['Tate_metrics']['short_time']['single'][variant] = {\
                        'trials': trials,\
                        'successes': successes,\
                        'inelasticity': inelas,\
                        'SE_inelasticity': SEinelas}
        trialS += " & " + str(int(u.sig_digits(trials, 3)))
        succS += " & " + str(int(u.sig_digits(successes, 3)))
        inelasS += " & " + str(u.sig_digits(inelas, 3))
        SEinelasS += " & " + str(u.sig_digits(SEinelas, 3))
    row1L.append(trialS)
    row1L.append(succS)
    row1L.append(inelasS)
    row1L.append(SEinelasS)

    # dict and JSON for Binary valued AB test analysis short
    print "\nShort time scale analysis, data for Binary valued A/B analysis"
    print "Bounce rate = 1 - inelasticity"
    print "variant, trials, successes, inelasticity, SE(inelasticity)"
    for varp in variantPairL:
        var1, var2 = varp
        pair = var1+'_'+var2
        N1, M1 = variantBinaryCountsShortD[var1]
        N2, M2 = variantBinaryCountsShortD[var2]
        mu1, mu2 = fn.mu_freq(N1, M1), fn.mu_freq(N2, M2)
        if mu1 >= mu2:
            varA = var1
            NA, MA = N1, M1
            varB = var2
            NB, MB = N2, M2
        else:
            varA = var2
            NA, MA = N2, M2
            varB = var1
            NB, MB = N1, M1
        print '\n', varA, NA, MA, round(fn.mu_freq(NA, MA), 2), round(sqrt(fn.var_mu_freq(NA, MA)), 4)
        print varB, NB, MB, round(fn.mu_freq(NB, MB), 2), round(sqrt(fn.var_mu_freq(NB, MB)), 4)
        outName = test_name + '_' + varA + '_' + varB + '_short'
        ABCD = {"supp": 0, "outName": outName\
            , "data": {"varA": varA, "varB": varB,\
            "NA": NA, "MA": MA, "NB": NB, "MB": MB}\
            , "metrics": {"ProbDiff": 0.05, "ProbLift": page_views_lift}}
        gname = '../calc/ABConfig' + outName + '.json'
        h = open(gname, 'w')
        h.write(json.dumps(ABCD))
        h.close()
        row0S = 'Landing Page Probability Lift & 0\% & '+varA+' & '+varB+' & '
        z = fn.zlift(NA, MA, NB, MB, 0)
        cl = fn.conf(z)
        print "For lift =", 0, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
        row0S += str(round(100.*cl, 1))

        rowlS = 'Landing Page Probability Lift & ' + str(100*page_views_lift)+'\% & '+varA+' & '+varB+' & '
        z = fn.zlift(NA, MA, NB, MB, page_views_lift)
        cl = fn.conf(z)
        if varA in outD['Tate_metrics']['short_time']['comp.']:
            pass
        else:
            outD['Tate_metrics']['short_time']['comp.'][varA] = {}
        outD['Tate_metrics']['short_time']['comp.'][varA][varB] = {\
                            'lift': page_views_lift,\
                            'z': z, 'conf': conf,\
                            'json_for_ABC': gname}                       
        print "For lift =", page_views_lift, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
        u.check_file(gname, "Wrote")
        print "\nRun calc_ab_stats.py on ", gname
        rowlS += str(round(100.*cl, 1))
        compairD[pair]['row2L'].append(rowlS)
        compairD[pair]['row2L'].append(row0S)
    
    # ####################### Late time scale for Binary AB test
    outD['Tate_metrics']['late_time'] = {}
    outD['Tate_metrics']['late_time']['single'] = {}
    outD['Tate_metrics']['late_time']['comp.'] = {}
    variantBinaryCountsLateD = {}

    trialS = "Website & Trials"
    succS = "Website & Successes"
    inelasS = "Website & Inelasticity"
    SEinelasS = "Website & SE(Inelasticity)"
    for variant in variantL:
        trials = sum(map(lambda i: variantPagesUsersD[variant][i][1], variantPagesUsersD[variant].keys()))- variantPagesUsersD[variant][1][1]
        successes = trials - variantPagesUsersD[variant][2][1]
        inelas = fn.mu_freq(trials, successes)
        SEinelas = fn.var_mu_freq(trials, successes)
        variantBinaryCountsLateD[variant] = (trials, successes)
        outD['Tate_metrics']['late_time']['single'][variant] = {\
                        'trials': trials,\
                        'successes': successes,\
                        'inelasticity': inelas,\
                        'SE_inelasticity': SEinelas}
        trialS += " & " + str(int(u.sig_digits(trials, 3)))
        succS += " & " + str(int(u.sig_digits(successes, 3)))
        inelasS += " & " + str(u.sig_digits(inelas, 3))
        SEinelasS += " & " + str(u.sig_digits(SEinelas, 3))
    row1L.append(trialS)
    row1L.append(succS)
    row1L.append(inelasS)
    row1L.append(SEinelasS)

    # dict and JSON for Binary valued AB test analysis LATE
    print "\nLate time scale analysis, data for Binary valued A/B analysis"
    print "Page Transition probability"
    print "variant, trials, successes, trans_prob, SE(trans_prob)"
    for varp in variantPairL:
        var1, var2 = varp
        pair = var1+'_'+var2
        N1, M1 = variantBinaryCountsLateD[var1]
        N2, M2 = variantBinaryCountsLateD[var2]
        mu1, mu2 = fn.mu_freq(N1, M1), fn.mu_freq(N2, M2)
        if mu1 >= mu2:
            varA = var1
            NA, MA = N1, M1
            varB = var2
            NB, MB = N2, M2
        else:
            varA = var2
            NA, MA = N2, M2
            varB = var1
            NB, MB = N1, M1
        print '\n', varA, NA, MA, round(fn.mu_freq(NA, MA), 2), round(sqrt(fn.var_mu_freq(NA, MA)), 4)
        print varB, NB, MB, round(fn.mu_freq(NB, MB), 2), round(sqrt(fn.var_mu_freq(NB, MB)), 4)
        outName = test_name + '_' + varA + '_' + varB + '_late'
        ABCD = {"supp": 0, "outName": outName\
            , "data": {"varA": varA, "varB": varB,\
                       "NA": NA, "MA": MA, "NB": NB, "MB": MB}\
            , "metrics": {"PageViewsDiff": 0.05, "PageViewsLift": page_views_lift}}
        gname = '../calc/ABConfig' + outName + '.json'
        h = open(gname, 'w')
        h.write(json.dumps(ABCD))
        h.close()
        row0S = 'Website Transition Probability Lift & 0\% & '+varA+' & '+varB+' & '
        z = fn.zlift(NA, MA, NB, MB, 0)
        cl = fn.conf(z)
        print "For lift =", 0, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
        row0S += str(round(100.*cl, 1))

        rowlS = 'Website Transition Probability Lift & ' + str(100*page_views_lift)+'\% & '+varA+' & '+varB+' & '
        z = fn.zlift(NA, MA, NB, MB, page_views_lift)
        cl = fn.conf(z)
        if varA in outD['Tate_metrics']['late_time']['comp.']:
            pass
        else:
            outD['Tate_metrics']['late_time']['comp.'][varA] = {}
        outD['Tate_metrics']['late_time']['comp.'][varA][varB] = {\
                            'lift': page_views_lift,\
                            'z': z, 'conf': conf,\
                            'json_for_ABC': gname}                       
        print "For lift =", page_views_lift, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
        u.check_file(gname, "Wrote")
        print "\nRun calc_ab_stats.py on ", gname
        rowlS += str(round(100.*cl, 1))
        compairD[pair]['row2L'].append(rowlS)
        compairD[pair]['row2L'].append(row0S)
    

    # ################### Late time scale REGRESSION

    outD['Tate_metrics']['regress'] = {}    
    outD['Tate_metrics']['regress']['single'] = {}
    outD['Tate_metrics']['regress']['comp.'] = {}
    # Write csv of minimum pages visited, cumulative users for each variant
    print "\n"
    print "\n*********** Late time scale analysis, Exponential Decay Regression"
    hdrS = ','.join(['v','U'])
    for var in variantPagesUsersD:
        fname = os.path.join(outS, var +"_4fit.csv")
        f = open(fname, 'w')
        f.write(hdrS)
        for pages in variantPagesUsersD[var].keys():
            if pages == 1:
                pass
            else:
                num_users = variantPagesUsersD[var][pages][1]
                if num_users == 0:
                    print "Error: Cum. users who've visited", pages, "minimum pages in variant", var, "=", num_users
                else:
                    f.write('\n'+str(pages) + ',' + str(num_users))
        f.close()
        u.check_file(fname, "For R, wrote "+ hdrS + " to:") 
        
    # write to script.R
    Rfile_name = "PageViewsFit.R"
    Rfull_file_name = os.path.join(outS, Rfile_name)
    RF = open(Rfull_file_name, 'w')
    RF.write("#!/usr/bin/env Rscript\n")
    for variant in variantUserPagesD:
        RTemplateF = open("R_fit_template.txt", "r")
        for line in RTemplateF:
            newline = line.replace("$Variant", variant)
            RF.write(newline)
        RF.write("\n")
        RTemplateF.close()

    # finish writing script.R and check for it
    RF.close()
    u.check_file(Rfull_file_name, "\nWrote") 
    
    # Call R for regression
    Rcmd = "R CMD BATCH " + Rfile_name
    Popen(Rcmd.split(' '), cwd = outS).wait()
    for var in variantPagesUsersD:
        fname = os.path.join(outS, "summary_" + var +".csv")
        u.check_file(fname, "R regressed and wrote to")

    
    # Summary stat.s dict for exp. decay model parameters
    variantStatsD = {}
    print "\nExp. Decay model parameters"
    print "variant, mean_page_views, SE_mean_pv"
    # pdb.set_trace()
    pvS = "Website Regression & mean pageviews "
    SEpvS = "Website Regression & SE(mean pageviews) "
    for var in variantL:
        fname = os.path.join(outS, "summary_" + var +".csv")
        # print fname
        f = open(fname, 'r')
        for line in f:
            lL = line.strip().split(',')
            if lL[0].strip('"') == 'pv':
                pv = float(lL[1].strip())
                SE_pv = float(lL[2].strip())
            else:
                pass
        variantStatsD[var]=[pv, SE_pv]
        print var, round(pv, 2), round(SE_pv,4)
        outD['Tate_metrics']['regress']['single'][var] = {\
                                'mean_page_views': pv,\
                                'SE_mean_page_views': SE_pv}
        pvS += " & " + str(u.sig_digits(pv, 3))
        SEpvS += " & " + str(u.sig_digits(SE_pv, 3))
    row1L.append(pvS)
    row1L.append(SEpvS)

    #print "hdr1S", hdr1S
    #print "row1L[0]", row1L[0]
    u.make_tex_table(outS + "descvartable", hdr1S, row1L)
    u.input_tex_table(tex_analysisF, "descvartable", "Descriptive Histogram stat.s for " + tex_test_name, "table:hist"+tex_test_name)
        
    
    # Inference A/B stat.s for page_views coefficient of variants (lift)
    gname = outS + "_Inf_Stats_Late.txt"
    g = open(gname, 'w')
    
    print "\nAverage page_views stat.s for exp decay model of late times"
    g.write("\nAverage page_views stat.s for exp decay model of late times")
    lift = page_views_lift
    lift0 = 0
    print "minimum page views lift =", lift
    g.write("\nminimum page views lift =" + str(lift))

    print "\nvariantA, variantB, pageviews_lift(%), z(lift A-B), confidence_level(%)"
    g.write('\nvariantA, variantB, pageviews_lift(%), z(lift A-B), confidence_level(%)')
    for varp in variantPairL:
        var1, var2 = varp
        pair = var1+'_'+var2
        if variantStatsD[var1][0] >= variantStatsD[var2][0]:
            varA, varB = var1, var2
        else:
            varA, varB = var2, var1

        z_pv = (variantStatsD[varA][0] - (1+lift)*variantStatsD[varB][0])\
            /sqrt(variantStatsD[varA][1]**2 + \
                  (1+lift)**2 * variantStatsD[varB][1]**2)
        cl_pv = fn.conf(z_pv)
        if varA in outD['Tate_metrics']['regress']['comp.']:
            pass
        else:
            outD['Tate_metrics']['regress']['comp.'][varA] = {}
        outD['Tate_metrics']['regress']['comp.'][varA][varB] = {\
                            'lift': lift, 'z': z_pv, 'conf': cl_pv}
        print varA, varB, lift*100, round(z_pv, 1), round(100.*cl_pv, 1)
        g.write('\n'+ varA +', ' +varB +', ' + str(lift*100) + ', ' + str(round(z_pv, 1)) + ', ' + str(round(100.*cl_pv, 1)))
        rowlS = 'Website Regressed PageViews Lift & '+str(lift*100)+'\% & '+varA+' & '+varB+' & '
        rowlS += str(round(100.*cl_pv, 1))
    
        z_pv0 = (variantStatsD[varA][0] - (1+lift0)*variantStatsD[varB][0])\
            /sqrt(variantStatsD[varA][1]**2 + \
                  (1+lift0)**2 * variantStatsD[varB][1]**2)
        cl_pv0 = fn.conf(z_pv0)
        if varA in outD['Tate_metrics']['regress']['comp.']:
            pass
        else:
            outD['Tate_metrics']['regress']['comp.'][varA] = {}
        outD['Tate_metrics']['regress']['comp.'][varA][varB] = {\
                            'lift': lift0, 'z': z_pv0, 'conf': cl_pv0}
        print varA, varB, lift0*100, round(z_pv0, 1), round(100.*cl_pv0, 1)
        g.write('\n'+ varA +', ' +varB +', ' + str(lift0*100) + ', ' + str(round(z_pv0, 1)) + ', ' + str(round(100.*cl_pv0, 1)))
        row0S = 'Website Regressed PageViews Lift & 0\% & '+varA+' & '+varB+' & '
        row0S += str(round(100.*cl_pv0, 1))
        compairD[pair]['row2L'].append(rowlS)
        compairD[pair]['row2L'].append(row0S)
    
    
    print "\n"
    g.close()
    u.check_file(gname, "Wrote Inferential stat.s for exp decay model for late times to:")

    #print outD
    gname = outS + 'all.json'
    g = open(gname, 'w')
    g.write(json.dumps(outD))
    g.close()
    u.check_file(gname, "Wrote ALL to:")
    
    for var in variantL:
            f = open(tex_analysisF, 'a')
            f.write("\\clearpage\n")
            f.write('\subsection{Figures for '+var+'}\n')
            f.write("See Fig.s~\\ref{fig:loghisto"+var+"} and \\ref{fig:log10minpv"+var+"} for a plot of the Credibility and Expected return vs. the values of " + var+".\n")
            f.close()
            figF = outD['Tate_metrics']['figures'][variant]['pv_log_histo'].split('/')[-1]
            u.input_tex_figure(tex_analysisF, figF, "log10(users) Page Views Histogram for" + var, "fig:loghisto"+var)
            
            figF = outD['Tate_metrics']['figures'][variant]['log10users_vs_min_pv'].split('/')[-1]
            u.input_tex_figure(tex_analysisF, figF, "log10(users) vs. minimum Page Views for" + var, "fig:log10minpv"+var)


    a = open(tex_analysisF, 'a')
    a.write("\\clearpage\n")
    a.write("\n\section{Comparison for all pairs}\n")
    a.close()
    for varp in variantPairL: # in outD["Freq_metrics"]["comp."]:
        varA, varB = varp
        pair = varA + '_' + varB
        a = open(tex_analysisF, 'a')
        a.write("\subsection{"+varA+" vs. "+varB+"}\n")
        a.write("\subsubsection{Summary Table}")
        a.write("See Table~\\ref{table:results"+varA+varB+"} for a summary of the results.\n")
        a.close()
        u.make_tex_table(outS + varA +"compare"+varB+"table", compairD[pair]["hdr2S"], compairD[pair]["row2L"])
        u.input_tex_table(tex_analysisF, varA+"compare"+varB+"table", "Results for "+varA+" vs. "+varB, "table:results"+varA+varB)

        a = open(tex_analysisF, 'a')
        a.write("\subsubsection{Frequentist based on Histogram}\n")
        a.write("Normality assumption used with the descriptive stat.s of the histogram.\n")
        a.write("See Fig.~\\ref{fig:conf"+varA+varB+"}.\n")
        a.close()
        figF = outD['Freq_metrics']['comp.'][pair]['conf.png'].split('/')[-1]
        u.input_tex_figure(tex_analysisF, figF, "Confidence vs. lift in mean histogram page views for " + varA + " compared to " + varB, "fig:conf"+varA+varB)



    

            
if __name__ == '__main__':
    main()

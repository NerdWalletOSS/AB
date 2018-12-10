#!/usr/bin/python                                                               
"PageViewsStats.py"

# import pdb
import json
import numpy as np
import os
import pandas as pd
import sys

from math import erf, sqrt, log10
from subprocess import call, Popen

sys.path.insert(0, os.path.abspath('..'))

from lib import utilities as u
from lib import ab_stats_functions as fn
from lib import stats_modules as sm


def Phi(z): # replace with fn.conf(z)
    """ parametrized confidence level,
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


def main():
    f_path = os.getcwd()

    if len(sys.argv) == 1:
        print "Usage: $python PageViewsStats.py test_name page_views_lift"
        print "Data expected in ./Data/[test_name].txt"
        print "page_views_lift > 0"
        print "using defaults 'FakeTest', 0.15"
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

    # read data and create data dictionary {variant: {userid: page_views}}
    fname = "./Data/" + test_name + ".txt"
    variantUserPagesD = {}
    lnum = 0
    uuidL = []
    u.check_file(fname, "Found data file")
    f = open(fname, 'r')
    splitchar = ','
    # Split on splitchar
    for line in f:
        if lnum == 0:
            print "Headers in line", str(lnum), ':', line
            headerL = line.strip().split(splitchar)
            if len(headerL) == 3:
                pass
            else:
                splitchar = '\t'
                headerL = line.strip().split(splitchar)
                if len(headerL) == 3:
                    pass
                else:
                    print "Unknown text separator in data file", fname, "exit"
                    sys.exit(0)
            lnum += 1
        else:
            lL = line.strip().split(splitchar)
            variant = lL[0]
            uuid = lL[1]
            if uuid in uuidL:
                # tested on FakeDataFail1/2 (Same or different variants).
                # Edit FailConfig
                print "Constraint failed: non-unique uuid =", uuid, ", exiting"
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
            variantUserPagesD[variant][uuid] = page_views
            lnum += 1 
    f.close()
    variantUserPagesD.keys()

    
    gname = os.path.join(outS, "SummaryFreqStats.txt")
    g = open(gname, 'w')
    
    num_users_data = lnum-1
    print "Number of users in data =", num_users_data
    g.write(test_name)
    g.write("\n\nNumber of users in data = "+ str(num_users_data))
    
    num_users_dict = 0
    for variant in variantUserPagesD:
        num_users_variant = len(variantUserPagesD[variant])
        print "Number of users in variant", variant, " =",\
            str(num_users_variant)
        num_users_dict += num_users_variant
    if num_users_dict == num_users_data:
        pass
    else:
        print "Consistency fail: num_users in data different from in variantUserPagesD, exiting"
        sys.exit(2)


    # Write data files for each variant for R
    hdrS = ','.join(headerL[1:])
    for variant in variantUserPagesD:
        fname = os.path.join(outS, variant +"_data.csv")
        f = open(fname, 'w')
        f.write(hdrS)
        for user in variantUserPagesD[variant]:
            f.write('\n'+user + ',' + str(variantUserPagesD[variant][user]))
        f.close()
        u.check_file(fname, "Wrote:" + hdrS + "to:")
        
    # start write script.R
    Rfile_name = "PageViewsPlots.R"
    Rfull_file_name = os.path.join(outS, Rfile_name)
    RF = open(Rfull_file_name, 'w')
    RF.write('#!/usr/bin/env Rscript\n')
    # RF.write('initial.dir<-getwd()\n')
    # RF.write('setwd("./Output/'+test_name+'")\n')

    u.check_file("R_plot_template.txt")
    for variant in variantUserPagesD:
        RTemplateF = open("R_plot_template.txt", "r")
        for line in RTemplateF:
            newline = line.replace("$Variant", variant)
            RF.write(newline)
        RF.write("\n")
        RTemplateF.close()


    # Calculate Frequentist descriptive stat.s for each variant
    variantFreqStatsD = {}
    for variant in variantUserPagesD:
        pagesL = variantUserPagesD[variant].values()
        num_users = len(variantUserPagesD[variant])
        mean_pages = sm.mean(pagesL)
        var_pages = sm.cov_p(pagesL)
        SD_pages = sqrt(var_pages)
        var_mean = var_pages/num_users
        SE_mean = sqrt(var_mean)
        variantFreqStatsD[variant] = [num_users, mean_pages, var_pages, var_mean, SE_mean]
        # print Frequentist descriptive stat.s values
        print "\nVariant =", variant 
        print "number of users =", num_users
        print "Mean page views =", round(mean_pages, 3)
        print "Var(Page Views) =", round(var_pages, 3)
        print "SD(Page Views) =", round(SD_pages, 2)
        print "Var(mean_page_views) =", round(var_mean, 5)
        print "SE(mean_page_views) =", round(SE_mean, 3)
        g.write("\n\nVariant = " + variant )
        g.write("\nnumber of users = " + str(num_users))
        g.write("\nMean page views = " + str(round(mean_pages, 3)))
        g.write("\nVar(Page Views) = " + str(round(var_pages, 3)))
        g.write("\nSD(Page Views) = " + str(round(SD_pages, 2)))
        g.write("\nVar(mean_page_views) = " + str(round(var_mean, 5)))
        g.write("\nSE(mean_page_views) = " + str(round(SE_mean, 3)))

    g.close()
    u.check_file(gname, "Wrote summary Freq. stat.s to:")

    print "\nSummary of Frequentist Stat.s"
    print "variant, mean_page_views, SE_mean_pv"
    for var in variantFreqStatsD:
        pv, SE_pv = variantFreqStatsD[var][1], variantFreqStatsD[var][4]
        print var, round(pv, 2), round(SE_pv, 4)
        

    # percent increase in number of page views deemed important
    deltaL = [x/20. for x in range(21)]

    variantL = variantUserPagesD.keys()
    nvar = len(variantL)
    variantPairD = {}
    for i in range(nvar-1):
        varA = variantL[i]
        variantPairD[varA] = {}
        for j in range(i+1, nvar):
            varB = variantL[j]
            variantPairD[varA][varB]=[]

    DeltaConfD = {}
    for varA in variantPairD:
        DeltaConfD[varA] = {}
        for varB in variantPairD[varA]:
            DeltaConfD[varA][varB]={}
            for delta in deltaL:
                zdel = ((variantFreqStatsD[varA][1] -
                         variantFreqStatsD[varB][1]*(1+delta))/
                        (sqrt(variantFreqStatsD[varA][4] +
                              variantFreqStatsD[varB][4]*(1+delta)**2)))
                DeltaConfD[varA][varB][delta] = (zdel, fn.conf(zdel))

            
    # Write csv of delta, z, confidence level for each variant-pair for R
    hdrS = ','.join(['delta','z', 'confidence_level'])
    for varA in variantPairD:
        for varB in variantPairD[varA]:
            fname = os.path.join(outS, varA +"_"+ varB +"_conf.csv")
            f = open(fname, 'w')
            f.write(hdrS)
            for delta in deltaL:
                z = DeltaConfD[varA][varB][delta][0]
                conf = DeltaConfD[varA][varB][delta][1]
                f.write('\n'+str(delta) + ',' + str(z) + ',' +\
                        str(conf))
            f.close()
            u.check_file(fname, "Wrote" + hdrS +"to:")
        
    # write confidence plots to script.R
    u.check_file("R_conf_template.txt")
    for varA in variantPairD:
        for varB in variantPairD[varA]:
            RTemplateF = open("R_conf_template.txt", "r")
            for line in RTemplateF:
                newline1 = line.replace("$varA", varA)
                newline = newline1.replace("$varB", varB)
                RF.write(newline)
            RF.write("\n")
            RTemplateF.close()

    # finish writing script.R
    RF.close()
    u.check_file(Rfull_file_name, "Wrote") 
    
    # Call R to create png
    Rcmd = "R CMD BATCH " + Rfile_name
    Popen(Rcmd.split(' '), cwd = outS).wait()

    # check for files created by R
    for variant in variantUserPagesD.keys():
        for ftype in ["_data.csv", "_hist.png", "_density.png"]:
            fname = os.path.join(outS, variant+ftype)
            u.check_file(fname, "R read csv or created png")
    
    for varA in variantPairD:
        for varB in variantPairD[varA]:
            pair = varA + "_" + varB + "_"
            for ftype in ["conf.csv", "z.png", "conf.png"]:
                fname = os.path.join(outS, pair+ftype)
                u.check_file(fname, "R read csv or created png")
    

    # ############# Tate metrics

    #Create page views histogram dictionary
    print "\n******* Starting non-Frequentist analysis ********"
    variantPagesUsersD = {}
    for variant in variantUserPagesD:
        variantPagesUsersD[variant] = {}

    for variant in variantUserPagesD:
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
    if num_users_dict == num_users_data:
        pass
    else:
        print "Consistency fail: num_users in data different from\
in variantPagesUsersD, exiting"
        sys.exit(2)

    # fill in blank page numbers with 0 users
    for variant in variantPagesUsersD:
        maxpages = max(variantPagesUsersD[variant].keys())
        pagesL = range(maxpages + 1)[1:]
        for pages in pagesL:
            if pages in variantPagesUsersD[variant]:
                pass
            else:
                variantPagesUsersD[variant][pages] = [0]
        
    # cumulative sum = number of users that had at least that many page views
    for variant in variantPagesUsersD:
        mp = max(variantPagesUsersD[variant].keys())
        variantPagesUsersD[variant][mp].append(variantPagesUsersD[variant][mp][0])
        mp -= 1
        while mp > 0:
            variantPagesUsersD[variant][mp].append(variantPagesUsersD[variant][mp][0] + variantPagesUsersD[variant][mp + 1][1])
            mp -= 1

    # log_10(num users) vs. min. pages visited 
    variantPagesLogUsersD = {}
    for variant in variantPagesUsersD:
        variantPagesLogUsersD[variant] = {}
        for pages in variantPagesUsersD[variant].keys():
            num_users = variantPagesUsersD[variant][pages][1]
            if num_users == 0:
                print "Error: Cum. users who've passed thru", pages, "pages in variant", variant, "=", num_users
            else:
                variantPagesLogUsersD[variant][pages] = log10(num_users)

    # plot histograms
    xhdr = "page_views"
    for variant in variantPagesUsersD:
        yhdr = "numUsers"
        oname = outS + variant
        fname = oname + '_'+ yhdr + '_vs_' + xhdr
        pgsL = variantPagesUsersD[variant].keys()
        pgsL.sort()
        numUsersL = []
        for pgs in pgsL:
            numUsersL.append(variantPagesUsersD[variant][pgs][1])
        u.make_jpeg(pgsL, numUsersL, oname, xhdr, yhdr)
        u.show_jpeg(fname)

    # plot log histograms
    xhdr = "page_views"
    for variant in variantPagesLogUsersD:
        yhdr = "log10users"
        oname = outS + variant
        fname = oname + '_' + yhdr + '_vs_' + xhdr
        pgsL = variantPagesLogUsersD[variant].keys()
        pgsL.sort()
        logUsersL = []
        for pgs in pgsL:
            logUsersL.append(variantPagesLogUsersD[variant][pgs])
        u.make_jpeg(pgsL, logUsersL, oname, xhdr, yhdr)
        u.show_jpeg(fname)

    # ############ short time scale
    variantBinaryCountsShortD = {}
    for variant in variantPagesUsersD:
        trials = variantPagesUsersD[variant][1][1]
        successes = variantPagesUsersD[variant][2][1]
        variantBinaryCountsShortD[variant] = (trials, successes)

    # dict and JSON for Binary valued AB test analysis short
    print "\nShort time scale analysis, data for Binary valued A/B analysis"
    print "Bounce rate = 1 - stickiness or inelasticity"
    print "variant, landers, clickers, inelasticity, SE(inelasticity)"
    for varA in variantPairD:
        NA, MA = variantBinaryCountsShortD[varA]
        print '\n', varA, NA, MA, round(fn.mu_freq(NA, MA), 2), "SE = ", round(sqrt(fn.var_mu_freq(NA, MA)), 4)
        for varB in variantPairD[varA]:
            NB, MB = variantBinaryCountsShortD[varB]
            outName = test_name + '_' + varA + '_' + varB + '_short'
            ABCD = {"supp": 1, "outName": outName\
                , "data": {"NA": NA, "MA": MA, "NB": NB, "MB": MB}\
                , "metrics": {"ProbDiff": 0.05, "ProbLift": 0.1}}
            gname = '../calc/ABConfig' + outName + '.json'
            h = open(gname, 'w')
            h.write(json.dumps(ABCD))
            h.close()
            print varB, NB, MB, round(fn.mu_freq(NB, MB), 2), "SE = ", round(sqrt(fn.var_mu_freq(NB, MB)), 4)
            z = fn.zlift(NA, MA, NB, MB, 0)
            cl = fn.conf(z)
            print "For lift =", 0, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
            z = fn.zlift(NA, MA, NB, MB, page_views_lift)
            cl = fn.conf(z)
            print "For lift =", page_views_lift, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
            u.check_file(gname, "Wrote")
    
    # ####################### Late time scale for Binary AB test
    variantBinaryCountsLateD = {}
    for variant in variantPagesUsersD:
        trials = sum(map(lambda i: variantPagesUsersD[variant][i][1], variantPagesUsersD[variant].keys()))- variantPagesUsersD[variant][1][1]
        successes = trials - variantPagesUsersD[variant][2][1]
        variantBinaryCountsLateD[variant] = (trials, successes)

    # dict and JSON for Binary valued AB test analysis medium
    print "\nLate time scale analysis, data for Binary valued A/B analysis"
    print "Bounce rate = 1 - stickiness or inelasticity"
    print "variant, landers, clickers, inelasticity, SE(inelasticity)"
    for varA in variantPairD:
        NA, MA = variantBinaryCountsLateD[varA]
        print '\n', varA, NA, MA, round(fn.mu_freq(NA, MA), 2), "SE = ", round(sqrt(fn.var_mu_freq(NA, MA)), 4)
        for varB in variantPairD[varA]:
            outName = test_name + '_' + varA + '_' + varB + '_med'
            NB, MB = variantBinaryCountsLateD[varB]
            ABCD = {"supp": 1, "outName": outName\
                , "data": {"NA": NA, "MA": MA, "NB": NB, "MB": MB}\
                , "metrics": {"PageViewsDiff": 0.05, "PageViewsLift": 0.1}}
            gname = '../calc/ABConfig' + outName + '.json'
            h = open(gname, 'w')
            h.write(json.dumps(ABCD))
            h.close()
            u.check_file(gname, "Wrote")
            print varB, NB, MB, round(fn.mu_freq(NB, MB), 2), "SE = ", round(sqrt(fn.var_mu_freq(NB, MB)), 4)
            z = fn.zlift(NA, MA, NB, MB, 0)
            cl = fn.conf(z)
            print "For lift =", 0, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
            z = fn.zlift(NA, MA, NB, MB, page_views_lift)
            cl = fn.conf(z)
            print "For lift =", page_views_lift, ": z =", round(z, 1), "conf. =", round(100.*cl, 1), '%'
    

    # ################### Late time scale REGRESSION
    
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
        u.check_file(fname, "Wrote"+ hdrS + "to:") 
        
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
    u.check_file(Rfull_file_name, "written") 
    
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
    for var in variantPagesUsersD:
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
        
    # Inference A/B stat.s for page_views coefficient of variants (lift)
    gname = outS + "_Inf_Stats_Late.txt"
    g = open(gname, 'w')
    
    print "\nAverage page_views stat.s for exp decay model of late times"
    g.write("\nAverage page_views stat.s for exp decay model of late times")
    lift = page_views_lift
    print "minimum page views lift =", lift
    g.write("\nminimum page views lift =" + str(lift))

    print "\nvariantA, variantB, z(lift A-B), confidence_level (%). For lift(%) =", lift*100
    g.write('\nvariantA, variantB, z(lift A-B), confidence_level (%). For lift(%) =", lift*100')
    for varA in variantPairD:
        for varB in variantPairD[varA]:
            z_pv = (variantStatsD[varA][0] - (1+lift)*variantStatsD[varB][0])\
                /sqrt(variantStatsD[varA][1]**2 + \
                      (1+lift)**2 * variantStatsD[varB][1]**2)
            cl_pv = fn.conf(z_pv)
            print varA, varB, round(z_pv, 1), round(100.*cl_pv, 1)
            g.write('\n'+ varA +', ' +varB +', ' + str(round(z_pv, 1)) + ', ' + str(round(100.*cl_pv, 1)))
            variantPairD[varA][varB].append(z_pv)
            variantPairD[varA][varB].append(cl_pv)
    # all above has to go into json
    

    g.close()
    u.check_file(gname, "Wrote Inferential stat.s for exp decay model for late times to:")

if __name__ == '__main__':
    main()

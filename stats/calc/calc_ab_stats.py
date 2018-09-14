#!/usr/bin/python                                                               
""" calc_ab_stats.py
Calculates and plots comparison metrics as a function of credibility for
results of an AB test

Args:
    NA = number of trials in variant A
    MA = number of successes in variant A
    MB = number of trials in variant B
    MB = number of successes in variant B

    DELTA = pA - pB (= 0.1)
    LIFT = pA/pB - 1 (= 0.2)
    ODDSFACTOR = pA/(1-pA)/(pB/(1-pB)) (= 1.2)
    
Returns:
    jpeg and csv of Bayesian credibility vs. comparison metric
    parametric and non-parametric credibilites for above threshold differences 
"""
import argparse
import json
import logging
# import numpy as np
import os
# import pandas as pd
import pdb
import sys
from math import sqrt, erf, exp
from scipy.stats import beta, norm
from subprocess import call, Popen

sys.path.insert(0, os.path.dirname(os.path.realpath(__file__))  + os.sep + os.pardir)

from lib import utilities as u
from lib import ab_stats_functions as fn

from metrics import metricsClass as mc
from metrics.metricsClass import metricC

def validate_data(dataD):
    # TODO Discuss with Ranjeet on why this
    # As long as the keys are there why do we care
    # if len(dataD) not in (4, 6):
    #     print "Unexpected number of arguments, 4 or 6 expected, exiting"
    #     sys.exit(1)
    for data_name in ("NA", "MA", "NB", "MB"):
        data_value = dataD[data_name]
        if type(data_value) != type(1):
            print "Invalid data_type", data_name, data_value, "exiting"
            sys.exit(0)

    NA = dataD["NA"]
    MA = dataD["MA"]
    NB = dataD["NB"]
    MB = dataD["MB"]
    
    # Check validity of input values
    if (NA <= 0 or MA < 0 or NB <= 0 or MB < 0):
        print "Negative count encountered, exiting"
        sys.exit(0)

    if (MA > NA or MB > NB):
        print "Inequalities in counts not satisfied, exiting"
        sys.exit(0)

    return NA, MA, NB, MB
    

def get_variant_names(dataD):
    # optional Variant names
    if 'varA' in dataD.keys():
        varA = dataD["varA"]
    else:
        varA = "TestSegment"

    if 'varB' in dataD.keys():
        varB = dataD["varB"]
    else:
        varB = "Control"

    return varA, varB

def get_metric_names():
    mtrCL = mc.metricC.metricsL
    mnameL = map(lambda i: i.name, mtrCL) #metrics in class
    return mnameL
    

def get_metric_min_values(metricsD):
    mtrCL = mc.metricC.metricsL
    mnameL = get_metric_names()
    
    minD = {} # minimum acceptable return
    for mkey in metricsD.keys():
        mname = mkey.strip()
        if (mname in mnameL):
            if (type(metricsD[mkey]) == type(0.1)):
                minD[mname] = metricsD[mkey]
            else:
                abLogger.error("Min. Acceptable Value for", mname, "not float")
                abLogger.error("Continuing with rest")
        else:
            abLogger.error("Config Error: metric", mkey, "in", configF)
            abLogger.error(mname, "not in metrics Class list")
            abLogger.error("Continuing with rest")

    # check valid range for minimum acceptable value of metrics
    for mtr in mtrCL:
        minm = mtr.lower_bound
        maxm = mtr.upper_bound
        mname = mtr.name
        # print mname, minm, maxm
        try:
            if minm <= minD[mname] <= maxm:
                pass
            else:
                abLogger.error(mname, "Min. Acceptable value=", minD.pop(mname))
                abLogger.error("out of range, continuing with rest")
        except KeyError:
            abLogger.error(mname, "not in config file")
            pass

    return minD


def initialize_output_directory(testname):
    # remove output files and (re)make directory
    outS = "./Output/" + testname + '/'
    rmS = "rm -fr " + outS
    mkS = "mkdir -p " + outS
    call(rmS.split(' '))
    call(mkS.split(' '))
    u.check_file(outS, "(re)made dir")
    return outS


def calc_freq_stats(NA, MA, NB, MB):
    # calculate the Frequentist descriptive stat.s
    # for both "A"=test and "B"=control
    # and the Frequentist confidence that A is better than B,
    # i.e. p_A > p_B
    MUA = u.sig_digits(fn.mu_freq(NA, MA), 3)
    VARMUA = u.sig_digits(fn.var_mu_freq(NA, MA), 2)
    MUB = u.sig_digits(fn.mu_freq(NB, MB), 3)
    VARMUB = u.sig_digits(fn.var_mu_freq(NB, MB), 2)
    ZPROP = u.sig_digits(fn.zlift(NA, MA, NB, MB, 0), 2)
    FREQ_CONF = u.sig_digits(fn.conf(ZPROP), 3)

    freqD = {'MUA': MUA, 'VARMUA': VARMUA, 'MUB': MUB, 'VARMUB': VARMUB, 'z': ZPROP, 'confidence': round(FREQ_CONF*100, 1)}
    return freqD


def get_Bayesian_pdf(p, trials, successes):
    N = trials
    M = successes
    return beta.pdf(p, M + 1, N - M + 1) 


def get_Bayesian_cdf(p, trials, successes):
    N = trials
    M = successes
    return beta.cdf(p, M + 1, N - M + 1) 


def calc_Bayes_stats(NA, MA, NB, MB, pAAr):
    # calculate the Bayesian descriptive stat.s
    # for both "A"=test and "B"=control
    # and the Bayesian Credibility that A is better than B,
    # i.e. p_A > p_B
    MUA = u.sig_digits(fn.mu_bayes(NA, MA), 3)
    VARMUA = u.sig_digits(fn.var_mu_bayes(NA, MA), 2)
    MUB = u.sig_digits(fn.mu_bayes(NB, MB), 3)
    VARMUB = u.sig_digits(fn.var_mu_bayes(NB, MB), 2)
    CRED = u.sig_digits(mc.probDiffM.calc_cred(NA, MA, NB, MB, pAAr, 0.), 3)

    bayesD = {'MUA': MUA, 'VARMUA': VARMUA, 'MUB': MUB, 'VARMUB': VARMUB, 'credibility': round(CRED*100, 1)}
    return bayesD


def get_Bayesian_prob_from_quantile(qq, trials, successes):
    """ For M successes from N trials, calculates the probability 
    at which the cumulative of the Bayesian Post. dist. has a 
    given quantile value qq
    PPF is the inverse of the CDF"""
    N = trials
    M = successes
    return beta.ppf(qq, M + 1, N - M + 1)


def get_Bayesian_probs_from_quantiles(quantile_range, num_quantiles, trials, successes):
    QQA = u.make_quantile_array(quantile_range, num_quantiles)
    return get_Bayesian_prob_from_quantile(QQA, trials, successes)
    
def setupLogging():
    abLogger = logging.getLogger()
    abLogger.setLevel(logging.NOTSET)
    # logLevel = options['logLevel']
    # if (logLevel=="info"):
    #     ll = logging.INFO    
    # elif (logLevel=="debug"):
    #     ll = logging.DEBUG
    # elif (logLevel=="warn"):
    #     ll = logging.WARNING
    # elif (logLevel=="err"):
    #     ll = logging.ERROR
    # elif (logLevel== "all"):
    #     ll = logging.NOTSET
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    
    fileLogger = logging.FileHandler('./ab.log') 
    fileLogger.setLevel(logging.INFO)
    fileLogger.setFormatter(formatter)
    abLogger.addHandler(fileLogger)

    consoleLogger = logging.StreamHandler()
    consoleLogger.setLevel(logging.ERROR)
    consoleLogger.setFormatter(formatter)
    abLogger.addHandler(consoleLogger)
    return abLogger

def getArgParser():
    parser = argparse.ArgumentParser(description="AB TEST result analyzer")
    parser = argparse.ArgumentParser(description='AB Test result calculator')
    # parser.add_argument("-v", "--verboseity", action="store", dest="logLevel", 
    #     choices={"info", "debug", "warn", "err", "all"}, default="err",
    #     help='Logging level required [default: %(default)s]')
    subparsers = parser.add_subparsers(help='sub-command help', dest="subparser_name" )
    parser_json = subparsers.add_parser("j", help="For JSON parsing")
    parser_json.add_argument(action="store", dest='json',
            help='Path to JSON config file [default: %(default)s]',
            default="ABConfig_rst.json", nargs='?')
    parser_list = subparsers.add_parser("l", help="List metrics registered")
    parser_list.add_argument(action="store_true", dest='list',
            help='When flag is set it just prints all the list of valid metrics',
            default=False)

    parser_metric = subparsers.add_parser("e", help="For metric calculations")
    parser_metric.add_argument("-m", "--metric", action="store", dest="metric",
            help='A metric name (as returned by -l) and the minimum acceptance value'
            , required=True, nargs=2 , metavar=('METRIC', 'MINVAL'))
    
    #groupM = group.add_argument_group()
    #parser_metric.add_argument(action="store", dest="metricRun",
    #        help='Select Action', choices=['cVSm', 'cred', 'decide'])
    group = parser_metric.add_mutually_exclusive_group(required=True)
    group.add_argument('-r', '--credibilityVSmetric', action='store_true',
            dest='cVSm', help='Create graph/csv for credibility vs metric '
            'or give a yes no answer (if no output is given)',
            default=False)
    group.add_argument('-d', '--credibility', action='store_true',
            dest='cred',
            help='output result for given metric value',
            default=False)
    #group.add_argument('-r', '--credibilityVSmetric', action='store_true',
    #        dest='cVSm', help='Create graph/csv for credibility vs metric',
    #        default=False)

    parser_metric.add_argument("-o", "--output", action="store", dest="output",
            help='File to give output in. Acceptable extensions are csv and jpeg'
            )
    parser_metric.add_argument(action="store", type=int, dest="NA",
            help='Number of trials for A')
    parser_metric.add_argument(action="store", type=int, dest="MA",
            help='Number of successes for A')
    parser_metric.add_argument(action="store", type=int, dest="NB",
                help='Number of trials for B')
    parser_metric.add_argument(action="store", type=int, dest="MB",
            help='Number of successes for B')
    parser_bayesian = subparsers.add_parser("b", help="Calculate Plain vanilla Bayesian Credibility Value")
    parser_bayesian.add_argument(action="store", type=int, dest="NA",
            help='Number of trials for A')
    parser_bayesian.add_argument(action="store", type=int, dest="MA",
            help='Number of successes for A')
    parser_bayesian.add_argument(action="store", type=int, dest="NB",
                help='Number of trials for B')
    parser_bayesian.add_argument(action="store", type=int, dest="MB",
            help='Number of successes for B')
    parser_frequentist = subparsers.add_parser("f", help="Calculate Plain vanilla Frequentist Confidence Value")
    parser_frequentist.add_argument(action="store", type=int, dest="NA",
            help='Number of trials for A')
    parser_frequentist.add_argument(action="store", type=int, dest="MA",
            help='Number of successes for A')
    parser_frequentist.add_argument(action="store", type=int, dest="NB",
                help='Number of trials for B')
    parser_frequentist.add_argument(action="store", type=int, dest="MB",
            help='Number of successes for B')
    return parser

def isDefault(parser, dest):
    if any (opt.dest == dest and (opt._long_opts[0] in sys.argv[1:] or
        opt._short_opts[0] in sys.argv[1:]) for opt in
        parser._get_all_options()):
            return True
    return False    

#def checkOptions(parser, options):
#    if options.json

def writeJson(folder, fName, jsonData):
    gname = os.path.join(folder, fName)
    with open(gname, 'w') as g:
        g.write(json.dumps(jsonData))
    abLogger.info("Wrote " + str(gname) )

abLogger = setupLogging()

def getConfigData(options):
   if 'list' in options.keys():
        for className in get_metric_names():
            print className
        sys.exit(0)    
   elif 'json' in options.keys():
        if not os.path.exists(options['json']):
            line = "Invalid JSON file given"
            abLogger.error(line)
            parser.error(line)
            sys.exit(1)
        with open(options['json'], "r") as f:
            return json.load(f)
   elif 'metric' in options.keys():
        # check if metric is in list of metrics
        if options['metric'][0] not in get_metric_names():
            line = "Metric " + options['metric'][0] + " not found"
            abLogger.error(line)
            parser.error(line)
            sys.exit(1)
        return makeConfigData(options) 

def makeConfigData(options):
    metric = {}
    if 'cred' in options.keys():
        metric[options['metric'][0]] = options['metric'][1]
    else:
        metric[options['metric'][1]] = 0

    dataDict = {}
    dataDict['NA'] = options["NA"]
    dataDict['MA'] = options["MA"]
    dataDict['NB'] = options["NB"]
    dataDict['MB'] = options["MB"]
    dataDict['VarA'] = "A"
    dataDict['VarB'] = "B"
    confD = {}
    confD['supp'] = 0
    if 'output' not in options.keys():
        confD['supp'] = 1
    else:
        lastStart= options['output'].rfind(".")
        if lastStart < 0:
            abLogger.error("Output file does not have an extension. Valid extensions are jpeg and csv")
            sys.exit(1)
        lastStart +=1
        if lastStart >= len(options['output']):
            abLogger.error("Output file does not have an extension. Valid extensions are jpeg and csv")
            sys.exit(1)
        ext = options['output'][lastStart:]
        if ext not in {"jpeg", "csv"}:
            abLogger.error("Output file does not have a valid extension. Valid extensions are jpeg and csv")
        confD['outFile'] = options['output']
        confD['ext'] = ext
    #confD['metrics']=
    confD['data'] = dataDict
    confD['outName'] = 'RST_default'
    return confD

def processParsedOptions(options):
    parser = options['subparser_name']
    if parser == 'l':
        for className in get_metric_names():
            print className
        sys.exit(0)    
    elif parser == 'j':
        if not os.path.exists(options['json']):
            line = "Invalid JSON file given"
            abLogger.error(line)
            sys.exit(1)
        with open(options['json'], "r") as f:
            # confD = 
            processJson(json.load(f))
    elif parser == 'e':
        # check if metric is in list of metrics
        if options['metric'][0] not in get_metric_names():
            line = "Metric " + options['metric'][0] + " not found"
            abLogger.error(line)
            sys.exit(1)
        elif  options['cred'] is True:
            if 'output' in options.keys() and options['output'] is not None:
                abLogger.error("Output file not expected for this configuration")
                parser.error("Output file not expected for this configuration")
                sys.exit(1)
            NA, MA, NB, MB = validate_data(options)
            PROBAAr = get_Bayesian_probs_from_quantiles(0.998, 999, NA, MA)
            mtrCL = mc.metricC.metricsL
            for mtr in mtrCL:
                mname = mtr.name
                if mname == options['metric'][0]:
                    CRED_M = mtr.calc_cred(NA, MA, NB, MB, PROBAAr, float(options['metric'][1]))
                    print CRED_M
                    sys.exit(0)
            #pdb.set_trace()
            line = "Metric not found " + options['metric'][0]
            abLogger.error(line)
            parser.error(line)
        else:
            # in this case we need to output a CSV or JPEG
            if options['cVSm'] == True:
                NA, MA, NB, MB = validate_data(options)
                PROBAAr = get_Bayesian_probs_from_quantiles(0.998, 999, NA, MA)
                PROBBAr = get_Bayesian_probs_from_quantiles(0.998, 999, NB, MB)
                mtrCL = mc.metricC.metricsL
                
                if 'output' not in options.keys() or options['output'] is None:
                    for mtr in mtrCL:
                        mname = mtr.name
                        if mname == options['metric'][0]:
                            maxmin, credL, mvalueL, exp_retL = mtr.get_expected_return(NA, MA, NB, MB, PROBAAr)
                            if float(options['metric'][1]) > maxmin:
                                print "NO"
                            else:
                                print "YES"
                            sys.exit(0)
                    abLogger.error("Metric not found")
                    sys.exit(1)

                
                for mtr in mtrCL:
                    mname = mtr.name
                    if mname == options['metric'][0]:
                        xhdr = mname
                        yhdr = 'Credibility'
                        ext, baseFName = getFileExtension(options['output'])
                        if ext not in {"jpeg", "csv"}:
                            abLogger.error("Output file does not have a valid extension. Valid extensions are jpeg and csv")
                            sys.exit(1)
                        if ext == "jpeg":
                            mvalueL ,credL = mtr.calc_cred_vs_metric(NA, MA, NB, MB, PROBAAr)
                            mtrName = mtr.name
                            fName = options['output']
                            #pdb.set_trace()
                            u.makeJpegNew(mvalueL, credL, baseFName, xhdr, yhdr, mname)
                        else:
                            fname = options['output']
                            mvalueL ,credL = mtr.calc_cred_vs_metric(NA, MA, NB, MB, PROBAAr)
                            #pdb.set_trace()
                            u.write_csv(mvalueL ,credL, baseFName, xhdr, yhdr)
        # return confD = 
        # return makeConfigData(options)
    elif parser == 'f':
        # just get n and m values calculated simple bayesian cred and exit
        NA, MA, NB, MB = validate_data(options)
        print calc_freq_stats(NA, MA, NB, MB)['confidence']
        sys.exit(0)
    elif parser == 'b':
        # just get n and m values calculated simple frequentist cred and exit
        NA, MA, NB, MB = validate_data(options)
        PROBAAr = get_Bayesian_probs_from_quantiles(0.998, 999, NA, MA)
        print calc_Bayes_stats(NA, MA, NB, MB,PROBAAr)['credibility']
        sys.exit(0)

def getFileExtension(fName):
    lastStart= fName.rfind(".")
    if lastStart < 0:
        abLogger.error("Output file does not have an extension. Valid extensions are jpeg and csv")
        sys.exit(1)
    lastStart +=1
    if lastStart >= len(fName) or lastStart<=0:
        abLogger.error("Output file does not have an extension. Valid extensions are jpeg and csv")
        sys.exit(1)
    return fName[lastStart:], fName[:lastStart-1]
    
def getCredibilityForMetric(mname, minAccD):
    ## default threshold = 1.1 X Min. Acceptable Expected return 
    mtrCL = mc.metricC.metricsL
    for mtr in mtrCL:
        if mname != mtr.name:
            continue
        if mname in minAccD:
            def_value = 1.1 * minAccD[mname]
            CRED_M = mtr.calc_cred(NA, MA, NB, MB, PROBAAr, def_value)
            line = "Cred[" + str(mname) + ">" + str(def_value) + "] =" +\
                                str(round(100.*CRED_M, 1)) +  "%"
            abLogger.info(line)
            myDict = {}
            myDict['value'] = def_value
            myDict['credibility'] = round(CRED_M*100., 1)
            return myDict
        else:
            abLogger.error(str(mname) + "not in minAccD")
        #print outD['Bayesian'][mname]
        return {}

def main(argv=None):
    
    parser = getArgParser()
    options = vars(parser.parse_args())
    #abLogger = setupLogging(options)
    f_path = os.getcwd()
    confD = processParsedOptions(options)


def processJson(confD):
    supp = confD['supp']
    test_name = confD['outName']
    labelL = test_name.split('_')[1:] # List of label items
    labelS = '_'.join(labelL)
    dataD = confD['data']
    NA, MA, NB, MB = validate_data(dataD)
    


    minAccD = get_metric_min_values(confD['metrics'])
    for mtr in minAccD:
        abLogger.info("Min Acceptable return for %s = %s" %(mtr,
            str(minAccD[mtr])))
    outS = initialize_output_directory(test_name)

    # Output Dictionary
    outD = {}
    outD['Frequentist'] = calc_freq_stats(NA, MA, NB, MB)
    line = "\n\nFrequentist Confidence Level\n"
    line = line + "Conf[A better than B] ={!s} % ".format(outD['Frequentist']['confidence'])
    abLogger.info(line)

    """ Bayesian Posterior distribution and properties
    Construct lists or arrays of p_A and p_B which span 99.8% of the volume
    of the Bayesian Posterior distribution
    for plotting or for numerical integration
    999 quantiles from 0.001 to 0.999
    These arrays and lists are CONSTANT
    Though the values do depend on NA, MA etc,
    they are fixed for the duration of the execution
    and have 'forgotten' about NA, MA, so might as well pass them
    instead of recalculating them """
    
    PROBAAr = get_Bayesian_probs_from_quantiles(0.998, 999, NA, MA)
    PROBBAr = get_Bayesian_probs_from_quantiles(0.998, 999, NB, MB)
                
    # plotBayesian(NA, MA, NB, MB, PROBAAr, PROBBAr, show=False)

    # Bayesian credibilities for 'simply better' and
    # default thresholds for each comparison metric
    outD['Bayesian'] = {}
    outD['Bayesian']['plain vanilla'] = calc_Bayes_stats(NA, MA, NB, MB,PROBAAr)
    line = "\n\nBayesian Credibility\n"
    line += "Cred[A better than B] = {!s} %".format(outD['Bayesian']['plain vanilla']['credibility'])
    abLogger.info(line)
    
    ## default threshold = 1.1 X Min. Acceptable Expected return 
    mtrCL = mc.metricC.metricsL
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            def_value = 1.1 * minAccD[mname]
            CRED_M = mtr.calc_cred(NA, MA, NB, MB, PROBAAr, def_value)
            line = "Cred[" + str(mname) + ">" + str(def_value) + "] =" +\
                                str(round(100.*CRED_M, 1)) +  "%"
            abLogger.info(line)
            print "Cred[", mname, ">", def_value, "] =",\
                                round(100.*CRED_M, 1), "%"
            outD['Bayesian'][mname] = {}
            outD['Bayesian'][mname]['value'] = def_value
            outD['Bayesian'][mname]['credibility'] = round(CRED_M*100., 1)
        else:
            abLogger.error(str(mname) + "not in minAccD")
        #print outD['Bayesian'][mname]


    # Construct lists or arrays of comparison metrics
    # which span 98% of the volume, 
    # percentiles or values of the CDF
    # percentileA = u.make_quantile_array(0.98, 99)


    ## Numerical calculation (Based on Bayesian distribution)
    ## of credibility as a function of metric

    # plot credibility (Bayesian) as a function of metric
    plot = 'credibility_vs_metric'
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            xhdr = mname
            yhdr = 'Credibility'
            xL, yL = mtr.plot_cred_vs_metric(NA, MA, NB, MB, PROBAAr, outS, labelS)
            fname = outS + yhdr + '_vs_' + xhdr
            u.write_csv(xL, yL, fname, xhdr, yhdr)
            outD['Bayesian'][mname][plot] = {}
            outD['Bayesian'][mname][plot]['jpeg'] = fname + '.jpeg'
            outD['Bayesian'][mname][plot]['csv'] = fname + '.csv'
            if supp == 1:
                u.show_jpeg(outD['Bayesian'][mname][plot]['jpeg'])
        else:
            pass
        #print mname, outD['Bayesian'][mname]


    ## plot expected minimum return as a function of metric, (not credibility)
    # find max(expected minimum return) and compare to min. acceptable value
    for mtr in mtrCL:
        mname = mtr.name
        if mname in minAccD:
            abLogger.info("For metric " + mname)
            print "\n\nFor metric", mname
            maxmin, credL, mvalueL, exp_retL = mtr.plot_expected_return(NA, MA, NB, MB, PROBAAr, outS, labelS)
            yL = exp_retL
            yhdr = 'Expected_Return'
            fname = outS + yhdr + "_vs_" + mname
            u.write_csv(mvalueL, yL, fname, mname, yhdr)
            if supp == 1:
                u.show_jpeg(fname)
            plot = 'exp_ret_vs_metric'
            outD['Bayesian'][mname][plot] = {}
            outD['Bayesian'][mname][plot]['jpeg'] = fname + '.jpeg'
            outD['Bayesian'][mname][plot]['csv'] = fname + '.csv'
            maxminS = "Experimental Max(Expected Return) = "+str(round(maxmin,3))
            minaccS = "Minimum Acceptable Return = " + str(minAccD[mname])
            if minAccD[mname] > maxmin:
                resultS = "NO"
            else:
                resultS = "YES"
            abLogger.info(maxminS)
            abLogger.info(minaccS)
            abLogger.info(maxminS)
            abLogger.info(resultS)
            print maxminS
            print minaccS
            print resultS
            outD['Bayesian'][mname]['maxmin'] = round(maxmin,3)
            outD['Bayesian'][mname]['minacc'] = minAccD[mname]
            outD['Bayesian'][mname]['result'] = resultS
        else:
            pass

    writeJson(outS, 'all.json', outD)


    def plotBayesian(NA, MA, NB, MB, PROBAAr, PROBBAr, show=False):
        # Calculate and plot the Bayesian post. probability distribution (pdf)
        # and cumulative function (cdf)

        BayesianVarD = {}
        PROBAL = PROBAAr.tolist()
        PROBBL = PROBBAr.tolist()
        pdfAL = get_Bayesian_pdf(PROBAAr, NA, MA).tolist()
        cdfAL = get_Bayesian_cdf(PROBAAr, NA, MA).tolist()
        pdfBL = get_Bayesian_pdf(PROBBAr, NB, MB).tolist()
        cdfBL = get_Bayesian_cdf(PROBBAr, NB, MB).tolist()
        PDFAD = {'list': pdfAL}
        CDFAD = {'list': cdfAL}
        PDFBD = {'list': pdfBL}
        CDFBD = {'list': cdfBL}
        BayesianVarD['A'] = {'xL': PROBAL, 'PDF': PDFAD, 'CDF': CDFAD}
        BayesianVarD['B'] = {'xL': PROBBL, 'PDF': PDFBD, 'CDF': CDFBD}
        varA, varB = get_variant_names(dataD)
        variantD = {}
        varL = ['A', 'B']
        variantD['A'] = {'name': varA}
        variantD['B'] = {'name': varB}
        abLogger.info("\nINPUT: A =%s B=%s" %(varA,varB))
        abLogger.info("NA, MA, NB, MB = %s %s %s %s\n"%( NA, MA, NB, MB))
        variantD['A']['N'] = NA
        variantD['A']['M'] = MA
        variantD['B']['N'] = NB
        variantD['B']['M'] = MB
        xhdr = 'True_Probability'
        for var in varL:
            for plot in ['PDF', 'CDF']:
                yhdr = 'Bayesian_' + plot
                oname = outS + variantD[var]['name'] + '_'
                fname = oname + yhdr + '_vs_' + xhdr
                u.write_csv(BayesianVarD[var]['xL'],\
                            BayesianVarD[var][plot]['list'], fname, xhdr, yhdr)
                BayesianVarD[var][plot]['csv'] = fname + '.csv'
                u.make_jpeg(BayesianVarD[var]['xL'],\
                            BayesianVarD[var][plot]['list'], oname,\
                            xhdr, yhdr, variantD[var]['name'])
                BayesianVarD[var][plot]['jpeg'] = fname + '.jpeg'

        if show:
            for var in BayesianVarD:
                for plot in ['PDF', 'CDF']:
                    fname = BayesianVarD[var][plot]['jpeg']
                    u.show_jpeg(fname)
        return BayesianVarD

    def plotCredibilityVSMetric(NA, MA, NB, MB, PROBAAr, minAccD, outPath, labelS, mname):
        plot = 'credibility_vs_metric'
    for mtr in mtrCL:
        if mname != mtr.name:
            continue
        if mname in minAccD:
            xhdr = mname
            yhdr = 'Credibility'
            xL, yL = mtr.plot_cred_vs_metric(NA, MA, NB, MB, PROBAAr, outS, labelS)
            fname = outS + yhdr + '_vs_' + xhdr
            u.write_csv(xL, yL, fname, xhdr, yhdr)
            outD['Bayesian'][mname][plot] = {}
            outD['Bayesian'][mname][plot]['jpeg'] = fname + '.jpeg'
            outD['Bayesian'][mname][plot]['csv'] = fname + '.csv'
            if supp == 1:
                u.show_jpeg(outD['Bayesian'][mname][plot]['jpeg'])
        else:
            pass
        #print mname, outD['Bayesian'][mname]

    
if __name__ == "__main__":
    main()

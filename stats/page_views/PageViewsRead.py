#!/usr/bin/python                                                               
"PageViewsRead.py"

import os
import sys
import numpy as np
import heapq
from subprocess import call
from math import erf, sqrt

#import stats_modules as sm
    

def main():

    if len(sys.argv) <2:
        print "Needs an argument: label of file with Page View data, exiting"
        sys.exit(2)
    else:
        label = "_"+sys.argv[1]

    fname = "./Data/_rst_page_view" + label
    f = open(fname, "r")
    uuid_pv_timesD={} # list of page_view times (secs from start date)
    lnum =0
    for line in f:
        lineL = line.split(',')
        pv_event_id = lineL[0].strip()
        page_sk = lineL[1].strip()
        time_secs = int(lineL[2].strip())
        RedshiftDTS = lineL[3].strip() #DateTime from Redshift
        uuid = lineL[4].strip()
        if uuid in uuid_pv_timesD:
            uuid_pv_timesD[uuid].append(time_secs)
        else:
            uuid_pv_timesD[uuid] = [time_secs]
        lnum+=1
    f.close()
    print "Number of page view events read = ", lnum

    for uuid in uuid_pv_timesD:
        uuid_pv_timesD[uuid] = sorted(uuid_pv_timesD[uuid])

    uuid_numpvD = {} # count of number of page views per user
    one_pv_uuidL = [] # uuid with only one page view
    zero_pv_uuidL = [] # uuid with no page view
    for uuid in uuid_pv_timesD:
        num_pv = len(uuid_pv_timesD[uuid])
        if num_pv == 0:
            zero_pv_uuidL.append(uuid)
        elif num_pv == 1:
            one_pv_uuidL.append(uuid)
        else:
            pass
        uuid_numpvD[uuid] = num_pv

    if len(zero_pv_uuidL) > 0:
        print len(zero_pv_uuidL), "uuid with 0 pageviews, removing from data"
        for uuid in zero_pv_uuidL:
            del uuid_pv_timesD[uuid]
            del uuid_numpvD[uuid]
    else:
        pass

    # sorted list of pageviews per user
    srtd_numpvL = sorted(uuid_numpvD.values(), reverse = True)
    rank = 0
    f = open("./Output/" + label +"_pv_Zipf.csv", "w")
    f.write("rank_of_user,num_pv")
    for pv in srtd_numpvL:
        rank += 1
        fS = "\n" + str(rank) + ", " + str(pv)
        f.write(fS)
    f.close()
    print rank, "users with non-zero pageviews"

    pv_numuuidD = {}
    for uuid in uuid_numpvD:
        pv = uuid_numpvD[uuid]
        if pv in pv_numuuidD:
            pv_numuuidD[pv] += 1
        else:
            pv_numuuidD[pv] = 1

    # sorted list of existing unique pageview counts
    pvL = sorted(pv_numuuidD.keys(), reverse = True)

    f = open("./Output/" + label +"_pv_counts.csv", "w")
    f.write("pv,num_uuid,cum_uuid,trans_prob")
    cum = 0
    cum_trials = 0
    for pv in pvL:
        cum_next = cum
        cum += pv_numuuidD[pv]
        tp = 1.*cum_next/cum
        fS = "\n" + str(pv) + ", " + str(pv_numuuidD[pv]) + ", " + str(cum) +\
             ", " + str(tp)
        f.write(fS)
    f.close()
    print cum, "Total users with non-zero pageviews"


    # start analysis of page view durations
    # remove uuid with 1 page view
    for uuid in one_pv_uuidL:
        del uuid_pv_timesD[uuid]

    num_durations = 0
    uuid_pv_durationD = {} # durations of page-views
    uuid_srtd_pv_durationD = {} # sorted in increasing duration
    for uuid in uuid_pv_timesD:
        uuid_pv_timesD[uuid].sort()
        t1A = np.array(uuid_pv_timesD[uuid][1:])
        t0A = np.array(uuid_pv_timesD[uuid][:-1])
        uuid_pv_durationD[uuid]= (t1A - t0A).tolist()
        uuid_srtd_pv_durationD[uuid] = sorted(t1A-t0A)
        num_durations += len(uuid_pv_durationD[uuid])

    rank = num_durations
    f = open("./Output/" + label+"_tdiff_Zipf.csv", "w")
    f.write("Rank,t_diff")
    for tdiff in heapq.merge(*uuid_srtd_pv_durationD.values()):
        fS = "\n" + str(rank) + ", " + str(tdiff)
        f.write(fS)
        rank -= 1
    f.close()

    duration_pagesD = {}
    for uuid in uuid_srtd_pv_durationD:
        for d in uuid_srtd_pv_durationD[uuid]:
            if d in duration_pagesD:
                duration_pagesD[d] += 1
            else:
                duration_pagesD[d] =1
    


    # Tate metrics



if __name__ == '__main__':
    main()

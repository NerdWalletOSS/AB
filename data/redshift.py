#!/usr/bin/env python
"""
interface to redshift
Basic how to use:
* make a connection object > rs=RedShift()
* have an sql string ready
* get result of query as a pandas DataFrame > df = rs.submit(sql)
* if you don't expect a result > df = rs.execute(sql)
* a few other convenient functions should be obvious on usage
  (is_open, close, table_check, table_drop)
configfile (default "redshift.confg") stores the connection string
that will contain your user/pass (for example in the __init__)
NOTES:
* What's deal with autocommit?  autocommit means table changes are committed
as they are run.  If doing complex things, you can initialize a RedShift object
with autocommit=False.  When you are ready, you can run the commit, or you can 
run rollback.
* Commandline... you can run this module with a sql file as an argument and it
will execute the file.
"""

import psycopg2
import sys, pprint, os, os.path
from datetime import date, timedelta, datetime, time
import logging
import pandas as pd
import useful
logr = logging.getLogger(__name__)


class RedShift:
    """
    manage connectino and submitting queries and retreiving results
    if connstring given, configfile is ignored
    """
    def __init__(self,configfile=None,connstring=None,connect=True,autocommit=True):
        # example strings
        #self.conn_string = "dbname='dev' port='5439' user='mshire' password='{}' host='east1-prod-redshift-0.chglg6f2aacl.us-east-1.redshift.amazonaws.com'".format("mypassword") #from prod-dwh-0
        #self.conn_string = "dbname='dev' port='5439' user='mshire' password='{}' host='redshift.nerdwallet.com'".format("mypassword)) #from laptop
        configpossibilities = ['/etc/redshift.config','/etc/redshift.conf','redshift.config','~/redshift.config']
        if connstring is None:
            if configfile is None:
                for cfg in configpossibilities:
                    if os.path.exists(cfg):
                        logr.warn("Using config file %s",cfg)
                        configfile=cfg
                        break
                logr.error("No configuration file found")
            with open(configfile) as ifp:
                self.conn_string = ifp.readline().strip()
        else:
            self.conn_string = connstring
        self.conn = None
        self.cursor = None
        self.autocommit = autocommit
        if connect:
            self.connect()
    def connect(self):
        logr.info('connecting to database')
        self.conn = psycopg2.connect(self.conn_string)
        self.cursor = self.conn.cursor()
        self.conn.autocommit = self.autocommit
        return (self.conn, self.cursor)
    def is_open(self):
        if self.conn:
            return (self.conn.closed==0)
        else:
            return False
    def close(self):
        logr.info('closing connection')
        self.conn.close()
        self.cursor = None
        self.conn = None
    def submit(self,query, close=False, fname=None, sep='\t', index=False):
        """
        submits a query (check if open, optionally close after, optionally save to file)
        returns a pandas dataframe of the results
        """
        if (not self.is_open()):
            self.connect()
        logr.debug(query)
        qdf = pd.read_sql(query, self.conn)
        if fname:
            qdf.to_csv(fname, sep=sep, index=index)
        if close:
            self.close()
        return(qdf)
    def execute(self, query, *args):
        """ for executing a query that isn't expected to return anything """
        if (not self.is_open()):
            self.connect()
        self.cursor.execute(query,*args)
    def commit(self):
        """ commit changes, but only if autocommit is not True """
        if not self.conn.autocommit:
            self.conn.commit()
    def rollback(self):
        """ rollback changes, but only if autocommit is not True """
        if not self.conn.autocommit:
            self.rollback()
    def table_check(self,tabname):
        """ convenience for seeing if a table exists """
        qry = "select relname from pg_class where relname = '{}';".format(tabname)
        qdf = self.submit(qry)
        return(len(qdf)>0)
    def table_drop(self,tabname):
        """ convenience for dropping a table """
        qry = "drop table if exists {};".format(tabname)
        self.execute(qry)

def make_conn_string(host,port,dbname,user,passwd):
    """
    make a redshift string
    """
    return( "dbname='{dbname}' port='{port}' user='{user}' password='{passwd}' host='{host}'".format(**locals()) )


def main():
    rs = Redshift()
    for fname in sys.argv[1:]:
        qry = open(fname).read()
        rs.execute(qry)
        

if __name__=='__main__':
    logging.basicConfig(level=10)
    main()
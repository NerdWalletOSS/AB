#!/usr/bin/python

import sys, os
import boto
from boto.s3.key import Key
import boto.s3.connection
from boto.s3.connection import S3Connection

def main():
    AWS_ACCESS_KEY_ID = 'AKIAJRSGWDICDAR5DYQQ' # AWS prod cred.s
    AWS_SECRET_ACCESS_KEY = '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'
    bucket_name = 'nerdwallet-dwh-redshift'

    #conn = boto.connect_s3(is_secure = False)
    conn = boto.s3.connect_to_region('us-east-1',
       is_secure=False               # uncomment if you are not using ssl
       )   
    # print conn

    mybucket = conn.get_bucket(bucket_name)

    # go through the list of files
    bucket_list = mybucket.list()
    for key in bucket_list:
        file_name = key.name.encode('utf-8')
        # print file_name
        # check if file exists locally, if not: download it
        if file_name == 'rt_000': # name of file desired
            if not os.path.exists(file_name):
                key.get_contents_to_filename(file_name)


if __name__ == '__main__':
    main()

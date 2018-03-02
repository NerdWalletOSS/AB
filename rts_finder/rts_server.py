#!/usr/bin/env python
import argparse
import boto.ec2
from collections import defaultdict
from flask import Flask
app = Flask(__name__)
import json
import os
import pdb
is_test=False

@app.route('/DescribeInstances')
@app.route('/DescribeInstances/<region>')
def describe_instances(region='us-east-1'):
    if is_test:
        return json.dumps(rts_server_dict)
    else:
        return describe_instances_not_test(region)

def describe_instances_not_test(region):
    ec2_conn = boto.ec2.connect_to_region(region)
    filters = {
	'tag:facet': 'ab-2017'
    }
    instances = ec2_conn.get_only_instances(filters=filters)
    result = defaultdict(list)
    for i in instances:
	result['server'].append(i.private_ip_address)
        result['port'].append(rts_port)
    return json.dumps(result)

def get_parser():
    parser = argparse.ArgumentParser(description="Flask Server to connect with RTS APP")
    parser.add_argument("-t", "--testing", action="store_true", dest='is_test',
            help="Set app to simply respond with settings in config",
            default=False)
    parser.add_argument("-f", "--config-file", action="store", dest='config_file',
            help="Set the path of the config file default[%default]",
            default="/opt/abadmin/db.json")
    return parser

def load_config(options):
    global is_test
    global rts_server_dict
    global rts_port
    rts_server_dict = defaultdict(list)
    with open(options.config_file, "r") as f:
        config = json.load(f)
        port = config['rts_finder_port']
        host = config['rts_finder_server']
        rts_server = config['ab_rts_server']
        rts_port = config['ab_rts_port']
    is_test = options.is_test or (len(options.multi_test) != 0)
    if len(options.multi_test) != 0 :
        for item in options.multi_test:
            values = item.split(':')
            if len(values) >2:
                sys.stderr.out('More than two elements in {}'.format(item))
                sys.exit(1)
            rts_server_dict['server'].append(values[0])
            rts_server_dict['port'].append(values[1])
    else:
        rts_server_dict['server'].append(rts_server)
        rts_server_dict['port'].append(rts_port)

    return host, port

if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter, description='RTS Finder')
    parser.add_argument("-t", "--testing", action="store_true", dest='is_test',
            help="Set app to simply respond with settings in config" )
    parser.add_argument('-T', '--multiserverTesting', action="store", default='',
            nargs="*",
            dest='multi_test', help="For testing: IP:PORTS that have RTS")
    parser.add_argument("-f", "--config-file", action="store", dest='config_file',
            default="/opt/abadmin/db.json",
            help="Set the path of the config file")
    options = parser.parse_args()
    host, port = load_config(options)
    app.run(host='0.0.0.0', port=int(port))

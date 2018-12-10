"""
   Request to try out
   curl -H "Content-type: application/json" -X POST --data @sample_input.json http://127.0.0.1:5000/classify
"""

import dt_interp
from flask import json, g
from flask import Flask, request
import time, logging


# initialize dt_interp
g_interp = dt_interp.init('../DT/spam', "random_forest")

app = Flask(__name__)


@app.before_request
def start_timer():
    g.start = time.time()


@app.after_request
def log_request(response):
    now = time.time()
    duration = round(now - g.start, 6)
    app.logger.debug("processing time : {} ms".format(duration*1000))
    return response


@app.route('/classify', methods = ['POST'])
def classify():
    if request.headers['Content-Type'] == 'application/json':
        json_body = json.dumps(request.get_json())
        result = dt_interp.classify(g_interp, json_body)
        return "JSON Message: " + result + "\n"
    else:
        return "415 Unsupported Media Type"

if __name__ == '__main__':
   app.run(host='0.0.0.0', port=5000, debug=True)


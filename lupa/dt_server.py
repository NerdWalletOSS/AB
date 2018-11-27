"""
   Request to try out
   curl -H "Content-type: application/json" -X POST --data @sample_input.json http://127.0.0.1:5000/classify
"""

import dt_interp
from flask import json
from flask import Flask, request


# initialize dt_interp
dt_interp.init('../DT/spam', "random_forest")

app = Flask(__name__)

@app.route('/classify', methods = ['POST'])
def classify():
    if request.headers['Content-Type'] == 'application/json':
        json_body = json.dumps(request.json)
        result = dt_interp.classify(json_body)
        return "JSON Message: " + result + "\n"
    else:
        return "415 Unsupported Media Type"

if __name__ == '__main__':
   app.run()


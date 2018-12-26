-- example HTTP POST script which demonstrates setting the
-- HTTP method, body, and adding a header

plfile = require 'pl.file'
plpath = require 'pl.path'
infile = "../DT/eg_xgboost/1_input.json"
assert(plpath.isfile(infile))
wrk = {}
wrk.body = plfile.read(infile)
wrk.method = "POST"
wrk.headers["Content-Type"] = " application/json"

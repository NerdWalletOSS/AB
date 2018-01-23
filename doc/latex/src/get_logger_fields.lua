plfile = require 'pl.file'
plpath = require 'pl.path'
filename = '../../../src/make_curl_payload.c'
assert(plpath.isfile(filename))
x = plfile.read(filename)
y = string.match(x, "START_COMMENT.-STOP_COMMENT*")
y = string.gsub(y, "/*START_COMMENT", "")
y = string.gsub(y, "STOP_COMMENT", "")
print(y)

print("ALL DONE")

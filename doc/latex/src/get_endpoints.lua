plfile = require 'pl.file'
plpath = require 'pl.path'

filename = '../../../src/ab_types.h'
assert(plpath.isfile(filename))
x = plfile.read(filename)
y = string.match(x, "_ab_req_type.~ AB_REQ_TYPE;")
print(x)
print(y)

print("ALL DONE")

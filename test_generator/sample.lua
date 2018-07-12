package.path=package.path .. ";./../src/?.lua"
j = require 'json'
x = require 'valid_config_generator'
y = require 'valid_complex_config2'
print(j.encode(x(y)))
-- pipe this to "json . "

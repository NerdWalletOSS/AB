local md5 = require 'lua/md5'
local function rand_name()
  return md5.sumhexa(tostring(math.random() * 100000000 ))
end
return rand_name

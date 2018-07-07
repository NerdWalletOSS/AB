local get_test_info = require 'test_webapp/get_test_info'
local function get_variant(test_id, vname)
  assert(test_id)
  assert(type(test_id) == "number")
  assert(vname)
  assert(type(vname) == "string")
  local V = {}
  local T = assert(get_test_info(test_id))
  local Variants = T.Variants
  for k, v in pairs(Variants) do 
    local is_found = false
    for k2, v2 in pairs(v) do 
      if ( k2 == "name" ) then 
        if ( v2 == vname ) then
          is_found = true
        end
      end
    end
    if ( is_found ) then return v end 
  end
  return nil
end
return get_variant
--[[ 
---- Sample usage of get_variant shown below
-- reset database and create a test 
T = require 'test_daft', T.t1(1)
-- get variant info for that test, which has id = 1
get_variant = require 'get_variant'
V = assert(get_variant(1, "Variant_B"))
print(V.percentage)
--]]

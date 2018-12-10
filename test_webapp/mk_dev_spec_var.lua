local JSON = require 'lua/JSON'

local function mk_dev_spec_var(
  Variants,
  percs
  )
  assert(type(Variants) == "table")
  assert(type(percs) == "table")
  local DxV = {}
  local D = assert(require 'test_webapp/devices')
  for device_id, v in pairs(D) do 
    local V = {}
    for k2, v2 in pairs(Variants) do
      V.id = v2.id
      V.device_id = device_id
      V.percentage = v2.percentage
      V.name = v2.name
    end
    DxV[v] = V
  end
  print(JSON:encode(DxV))
end
return mk_dev_spec_var

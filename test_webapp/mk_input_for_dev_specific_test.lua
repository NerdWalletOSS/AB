local R             = require 'test_webapp/rand_selections'
local function mk_input_for_dev_specific_test(tid, D, V, nV)
  local out = {}
  local X = {}
  for device_id, d in pairs(D) do 
    local t = {}
    local P = assert(R.rand_perc("XYTest", nV))
    for k, v in pairs(V) do
      t[k] = {}
      assert(P[k], " k = " .. k .. " #P = " .. #P)
      t[k].percentage = P[k]
      assert(device_id)
      t[k].device_id = device_id
      assert(v.id)
      t[k].variant_id = assert(tonumber(v.id))
    end
    X[d] = t
  end
  out.DeviceCrossVariant = X
  out.id = tid
  return out
end
return mk_input_for_dev_specific_test


require 'lua/str'
local JSON    = require 'lua/JSON'
local curl    = require 'lua/curl'
local R       = require 'test_webapp/rand_selections'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'

local server = os.getenv("WEBAPP_SERVER") or "localhost"
local tbp_url = "http://" .. server .. ":8080/AB/php/endpoints/endpoint_test_basic.php"
local adp_url = "http://" .. server .. ":8080/AB/php/endpoints/endpoint_add_addnl_var_info.php"

local function mk_rand_test(
  X
  )
  local T1 = R.rand_test(X)
  local V1 = assert(T1.Variants)
  local hdrs, outbody, status = curl.post(tbp_url, nil, JSON:encode(T1))
  assert(status == 200 )
  local tid = get_test_id(hdrs)
  -- check that basic data got added as required
  local chk_T1  = get_test_info(tid)
  assert(T1.name == chk_T1.name)
  assert(#T1.Variants == #chk_T1.Variants)
  if ( T1.description ) then 
    assert(T1.description == chk_T1.description)
  else
    assert(not chk_T1.description)
  end

  -- V2 has id and V1 has custom_data/description, so bring stuff into V2
  local T2  = get_test_info(tid)
  local V2 = assert(T2.Variants)
  for k2, v2 in ipairs(V2) do 
    for k1, v1 in ipairs(V1) do 
      if ( v1.name == v2.name ) then 
        V2[k2].custom_data = v1.custom_data
        V2[k2].description = v1.description
      end
    end
  end
  for k2, v2 in ipairs(V2) do 
    if ( ( v2.custom_data ) or ( v2.description ) )  then 
      local A = {}
      A.id = tid
      A.Updater = R.rand_admn()
      A.VariantID = assert(v2.id)
      A.Variants = V2
      local hdrs, outbody, status = curl.post(adp_url, nil, JSON:encode(A))
      -- for k, v in pairs(hdrs) do print(k,v ) end
      -- print(outbody)
      assert(status == 200 )
    end
  end
  -- check that custom data/description got added as required
  local T3  = get_test_info(tid)
  local V3 = assert(T3.Variants)
  for k3, v3 in ipairs(V3) do 
    for k1, v1 in ipairs(V1) do 
      if ( v1.name == v3.name ) then 
        if ( v1.custom_data ) then 
          if( V3[k3].custom_data ~= v1.custom_data ) then
            print("3", string.len(V3[k3].custom_data), V3[k3].custom_data)
            print("1", string.len(v1.custom_data), v1.custom_data)
          end
          assert( V3[k3].custom_data == v1.custom_data )
        end
        if ( v1.description ) then 
          assert( V3[k3].description == v1.description )
        end
      end
    end
  end
  --=== Add filters TODO  P2
  --=== Add device cross variant  TODO  P1
  return tid
end
return mk_rand_test

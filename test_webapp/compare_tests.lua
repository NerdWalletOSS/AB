local function eq_kv(T1, T2, k)
  assert(T1[k], "T1 key not found" .. k)
  assert(T2[k], "T2 key not found" .. k)
  assert(T1[k] == T2[k])
end
local function eq_if_set(T1, T2, k)
  if ( ( T1.k ) and ( T2.k ) ) then assert(T1.k == T2.k) end
end
--[[ Figure out how to implement lkp in lua 
local function set_id(T, key_tbl, k1, k2)
  if ( T[k1] ) then
    T[k2] = lkp(key_tbl, k1)
    print("setting key " .. k2 )
  else
    print("key not found " .. k1)
  end
end
==]]
local function compare_tests(T1, T2)
  assert(T1)
  assert(T2)
  assert(type(T1) == "table")
  assert(type(T2) == "table")
  assert(T1["Creator"])
  assert(T1["TestType"])
  --[[
  set_id(T1, "admin", "Creator", "creator_id")
  set_id(T1, "test_type", "TestType", "test_type_id")
  --]]
  eq_if_set(T1, T2, "id")
  eq_if_set(T1, T2, "name")
  eq_if_set(T1, T2, "test_type_id")
  eq_if_set(T1, T2, "bin_type_id")
  eq_if_set(T1, T2, "pred_id")
  eq_if_set(T1, T2, "channel_id")
  eq_if_set(T1, T2, "description")
  eq_if_set(T1, T2, "is_dev_specific")
  eq_if_set(T1, T2, "has_filters")
  eq_if_set(T1, T2, "seed")
  eq_if_set(T1, T2, "external_id")
  eq_if_set(T1, T2, "creator_id")
  eq_if_set(T1, T2, "state_id")
  return true
end
return compare_tests

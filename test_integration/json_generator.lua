package.path=package.path .. ";./../src/?.lua"
-- local dbg = require 'debugger'
require 'str'
math.randomseed(os.time())

local gen_table = {}
function gen_table.add_set_entry(j_table, constraints)
  local entry = {}
  for k,v in pairs(j_table.entry_fields) do
    if constraints.sum[k] ~= nil then
      -- STRONG ASSUMPTION is that the lower value is 0
      assert(v.type == "number", "The sum constraint can only be applied to numbers")
      assert(v.value.random ~= nil, "Sum constraint can only be applied to random")
      local old_upper = assert(tonumber(v.value.random.upper), "Must be a valid number")
      v.value.random.upper = constraints.sum[k]
      local n_v = gen_table.get_entry(v)
      print("constraint: ", k, n_v )
      local val = assert(tonumber(n_v), "must be  a number")
      assert( val <=tonumber(constraints.sum[k]))

      constraints.sum[k] = tostring(tonumber(constraints.sum[k]) - val)
      entry[k] = n_v
    elseif constraints.unique[k] ~= nil then
      local val
      repeat
        val = gen_table.get_entry(v)
      until constraints.unique[k][val] == nil
      constraints.unique[k][val] = true
      print("constraint: ", k, val)
      entry[k] = val
    else
      entry[k] = gen_table.get_entry(v)
    end
  end
  return entry
end

function gen_table.get_constraints(j_table)
  local constraints = {}
  for k,v in pairs(j_table.constraints) do
    if v.type == "sum" then
      local sum = constraints.sum or {}
      sum[k] = tostring(assert(tonumber(v.value), "Must have a valid sum total"))
      constraints.sum = sum
    elseif v.type == "unique" then
      local unique = constraints.unique or {}
      unique[k] = {} --, "Must have a valid sum total"
      constraints.unique = unique
    else
      print(k, v.type)
      error("Unknown type of constraint")
    end
  end
  return constraints
end

function gen_table.is_done(constraints)
  -- Currently only works on sume constraints
  for _,v in pairs(constraints.sum) do
    if tonumber(v) ~= 0 then
      return false
    end
  end
  return true
end

function gen_table.add_set(j_table)
  local set = {}
  local constraints = gen_table.get_constraints(j_table)

  repeat
    set[#set + 1] = gen_table.add_set_entry(j_table, constraints)
  until gen_table.is_done(constraints) == true

  return set
end


function gen_table.get_entry(v)
  if v.type == "list" then
    local index = math.random(#v.values)
    return v.values[index]
  elseif v.type == "regex" then
    local upper = assert(tonumber(v.length.random.upper), "Invalid upper bound")
    local lower = assert(tonumber(v.length.random.lower), "Invalid lower bound")
    local length = math.random(lower, upper)
    return string.random(length, v.regex)
  elseif v.type == "number" then
    local upper = assert(tonumber(v.value.random.upper), "Invalid upper bound")
    local lower = assert(tonumber(v.value.random.lower), "Invalid lower bound")
    local val
    if v.float == true then
      val = math.random(lower*100, upper*100)/100
    else
      val = math.random(lower, upper)
    end
    if val < 0 then
      print("Boooooo", val, upper, lower)
      error()
    end
    return tostring(val)
  elseif v.type == "set" then
    return gen_table.add_set(v)
  else
    error("Unknown type present")
  end

end

function gen_table.gen_table(l_table)
  local j_res = {}
  for k,v in pairs(l_table) do
    -- print(k, v)
    j_res[k] = gen_table.get_entry(v)
  end
  return j_res
end

return gen_table.gen_table

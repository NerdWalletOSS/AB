package.path=package.path .. ";./../src/?.lua"
-- local dbg = require 'debugger'
require 'str'
math.randomseed(os.time())

local gen_table = {}

function gen_table.get_exactly_one(k, constraints)
  if constraints.exactly_one[k] == nil then return nil end
  local target = nil
  local ex_one = constraints.exactly_one[k]
  for k2,v in pairs(ex_one) do
    if target == nil and v == false then
      target = k2
    end
  end
  if target ~= nil then
    constraints.exactly_one[k][target] = true
  end
 print("exactly one  constraint: ", k, target)
  return target
end

function gen_table.get_unique(k, v, constraints, init_value)
  local val
  local ex_one = constraints.exactly_one[k] or {}
  local unique = constraints.unique[k]
  
  if init_value ~= nil and unique[init_value] == nil then
    val = init_value
  end
  
  if val == nil then
    repeat
      val = gen_table.get_entry(v)
    until unique[val] == nil and ex_one[val] == nil
  end
  unique[val] = true
  print("unique constraint: ", k, val)
  return val
end

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
      print("sum constraint: ", k, n_v )
      local val = assert(tonumber(n_v), "must be  a number")
      assert(val <= tonumber(constraints.sum[k]))

      constraints.sum[k] = tostring(tonumber(constraints.sum[k]) - val)
      entry[k] = n_v
    elseif constraints.exactly_one[k] ~= nil then
      -- Slightly different from unique as in only the listed values should
      -- occire only once, the rest can occur multiple times too
      -- TODO come up with better logic, but for now we take a 50% chance to
      -- come up with the exactly once field
      local val
      if math.random(2) == 1 then
        val = gen_table.get_exactly_one(k, constraints)
      end
      if val == nil then
        repeat
          val = gen_table.get_entry(v)
        until constraints.exactly_one[k][val] == nil -- Nothing to do with the keys specified
      end
      if constraints.unique[k] ~= nil then
        entry[k] = gen_table.get_unique(k,v, constraints, val)
      else
        entry[k] = val
      end

    elseif constraints.unique[k] ~= nil then
      entry[k] = gen_table.get_unique(k,v, constraints)
    else
      entry[k] = gen_table.get_entry(v)
    end
  end
  return entry
end

function gen_table.get_constraints(j_table)
  local constraints = {}
  for k,e_set in pairs(j_table.constraints) do
    for _, v in ipairs(e_set) do
      if v.type == "sum" then
        local sum = constraints.sum or {}
        sum[k] = tostring(assert(tonumber(v.value), "Must have a valid sum total"))
        constraints.sum = sum
      elseif v.type == "unique" then
        local unique = constraints.unique or {}
        unique[k] = {} --, "Must have a valid sum total"
        constraints.unique = unique
      elseif v.type == "exactly_one" then
        local exactly_one = constraints.exactly_one or {}
        exactly_one[k] = {}
        for __, entry  in ipairs(v.values) do
          exactly_one[k][entry] = false
        end
        constraints.exactly_one = exactly_one
      else
        print(k, v.type)
        error("Unknown type of constraint")
      end
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


function gen_table.get_entry(v, exactly_one)
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

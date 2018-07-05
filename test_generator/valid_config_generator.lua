-- local dbg = require 'debugger'
require 'lua/str'
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
  -- print("exactly one  constraint: ", k, target)
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
  -- print("unique constraint: ", k, val)
  return val
end

function gen_table.add_set_entry(j_table, constraints)
  local entry = {}
  local is_last = false
  if constraints.count ~= nil then
    -- print("count constraint", constraints.count)
    if constraints.count == 1 then
      is_last = true
    end
    constraints.count = constraints.count - 1
  end
  -- dbg()
  for k,v in pairs(j_table.entry_fields) do
    if constraints.sum ~= nil and  constraints.sum[k] ~= nil then
      -- STRONG ASSUMPTION is that the lower value is 0
      assert(v.type == "number", "The sum constraint can only be applied to numbers")
      assert(v.value.random ~= nil, "Sum constraint can only be applied to random")
      if is_last then
        entry[k] = constraints.sum[k]
        constraints.sum[k] = tostring(0)
      else
        local old_upper = assert(tonumber(v.value.random.upper), "Must be a valid number")
        v.value.random.upper = constraints.sum[k]
        local n_v = gen_table.get_entry(v)
        -- print("sum constraint: ", k, n_v )
        local val = assert(tonumber(n_v), "must be  a number")
        assert(val <= tonumber(constraints.sum[k]))

        constraints.sum[k] = tostring(tonumber(constraints.sum[k]) - val)
        entry[k] = n_v
      end
    elseif constraints.exactly_one ~= nil and  constraints.exactly_one[k] ~= nil then
      -- Slightly different from unique as in only the listed values should
      -- occire only once, the rest can occur multiple times too
      -- TODO come up with better logic, but for now we take a 50% chance to
      -- come up with the exactly once field
      local val
      val = gen_table.get_exactly_one(k, constraints)

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
    elseif constraints.unique ~= nil and constraints.unique[k] ~= nil then
      entry[k] = gen_table.get_unique(k,v, constraints)
    else
      entry[k] = gen_table.get_entry(v)
    end
  end
  return entry
end

function gen_table.get_constraints(j_table)
  local constraints = {}
  -- dbg()
  for k,e_set in pairs(j_table.constraints) do
    -- print("constraint keys", k)
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
      elseif v.type == "count" then
        local count = constraints.count or 0
        count = math.random(
        tonumber(v.value.random.lower), tonumber(v.value.random.upper))
        constraints.count = count
        -- print(k, v.type, count)
      else
        error("Unknown type of constraint")
      end
    end
  end
  return constraints
end

function gen_table.is_done(constraints)
  -- Currently only works on sume constraints
  if constraints.sum ~= nil then
    for _,v in pairs(constraints.sum) do
      if tonumber(v) ~= 0 then
        return false
      end
    end
  end
  return true
end

function gen_table.add_set(j_table)
  local set = {}
  local constraints = gen_table.get_constraints(j_table)
  if constraints.count ~= nil then
    local count = constraints.count
    for i=1, count do
      set[#set + 1] = gen_table.add_set_entry(j_table, constraints)
    end
    assert(gen_table.is_done(constraints) == true, "Constraints should have been satisfied unless some constraints clobber each other")
  else
    repeat
      set[#set + 1] = gen_table.add_set_entry(j_table, constraints)
    until gen_table.is_done(constraints) == true
  end
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
      -- print("Boooooo", val, upper, lower)
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

function hack_for_ab_tests(l_table)
  local j_res = gen_table.gen_table(l_table)
  if j_res.State ~= "terminated" then
    for _, entry in ipairs(j_res.Variants) do
      entry.is_final = "0"
    end
  end

  if j_res.TestType == "ABTest" then
    -- Just a hack around the generator for ABTests 100/(nV-1)
    local nV = #j_res.Variants
    local percentage_left = 0
    local control_entry = nil
    local max_percentage = 100.0/(nV - 1)
    for _, entry in ipairs(j_res.Variants) do
      if entry.name ~= "Control" then
        local percentage = tonumber(entry.percentage)
        if percentage > max_percentage then
          entry.percentage = tostring(max_percentage)
          percentage_left = percentage_left + (percentage - max_percentage)
        end
      else
        control_entry = entry
      end
    end
    if control_entry ~= nil then
      control_entry.percentage = tostring(tonumber(control_entry.percentage) + percentage_left)
    end
  end
  return j_res
end

-- return gen_table.gen_table
return hack_for_ab_tests

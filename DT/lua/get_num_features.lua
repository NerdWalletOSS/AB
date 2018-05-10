package.path = '../../?.lua;../?.lua;' .. package.path 

local cache = require 'lua.cache'

local function get_nested_length(table)
  --[[
  Get the number of nested elements in a table. I.e. if a table contains a k, v pair
  with v as a table, counts the number of nested elements in that, and adds to the current count.
  ]]--
  local counter = 0
  for k, v in pairs(table) do
    if type(v) == 'table' then
      counter = counter + get_nested_length(v)
    else
      counter = counter + 1
    end
  end
  return counter
end

local function get_num_features()
  local dt_feature = assert(cache.get("dt_feature"),
    'dt_feature not in cache.')
  return get_nested_length(dt_feature)
end

return get_num_features

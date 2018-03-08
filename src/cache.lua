-- A permanent kv store that holds values till its explictly deleted. In the
-- future we will also use weak tables to make it a cache that can be garbage
-- collected in case memory pressure increased

local cache = {}
local c_store, c_weak_store

local function init()
  c_store = {}
  c_weak_store = {}
  setmetatable(c_weak_store, { __mode = 'v' })
end

function cache.put(key, value, weak)
  if weak == nil then weak = false end
  
  if weak == true then
    c_store[key]= value
  else
    c_weak_store[key]= value
  end
  return true
end

function cache.get(key)
  local value = c_store[key]
  if value == nil then
    return c_weak_store[key]
  else
    return value
  end
end

function cache.delete(key)
  c_store[key] = nil
  c_weak_store[key] = nil
end


function cache.purge()
  init()
end

init()
return cache

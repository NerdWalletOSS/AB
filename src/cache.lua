-- A permanent kv store that holds values till its explictly deleted. In the
-- future we will also use weak tables to make it a cache that can be garbage
-- collected in case memory pressure increased

local cache = {}
local c_store, c_weak_store

local function init()
  c_store = {}
  c_weak_store = {}
  setmetatable(c_weak_store, { __mode = 'kv' })
end

function cache.put(key, value, weak)
  if weak ~= true then weak = false end
  
  if weak == false then
    c_store[key]= value
  else
    c_weak_store[key]= {value}
  end
  return true
end

function cache.get(key)
  local value = c_store[key]
  if value == nil then
    local val = c_weak_store[key]
    if val ~= nil then 
      return val[0]
      else
      return nil
    end
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

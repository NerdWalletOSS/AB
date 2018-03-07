-- A permanent kv store that holds values till its explictly deleted. In the
-- future we will also use weak tables to make it a cache that can be garbage
-- collected in case memory pressure increased

local cache = {}
c_store = {}
function cache.put(key, value)
  c_store[key]= value
end

function cache.get(key)
   return c_store[key]
 end

function cache.delete(key)
    c_store[key] = nil
 end


function cache.purge()
  c_store = {}
end

return cache

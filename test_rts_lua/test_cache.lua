local cache = require 'lua/cache'

local function is_same(a, b)
  local val = type(a) == type(b)
  if not val then return false end
  if type(a) == "table" then
    for k,v in pairs(a) do
      if not is_same(v, b[k]) then return false end
    end
    for k,v in pairs(b) do
      if not is_same(v, a[k]) then return false end
    end
    return true
  else
    return a == b
  end
end

local function copy(obj, seen)
  if type(obj) ~= 'table' then return obj end
  if seen and seen[obj] then return seen[obj] end
  local s = seen or {}
  local res = setmetatable({}, getmetatable(obj))
  s[obj] = res
  for k, v in pairs(obj) do res[copy(k, s)] = copy(v, s) end
  return res
end
local function test_insert(key, value)
  cache.put(key, value)
  local val = cache.get(key)
  assert(is_same(val, value), "Cache should have the same value")
  cache.delete(key)
  val = cache.get(key)
  assert(val == nil, 'Value should be removed from cache on delete')
end




describe('Cache ', function()
  describe('for regular storage should give the same output as stored for types and be deleted when delete is called', function()
    it("numbers", function()
      test_insert(1, 1)
      test_insert(1, "five")
      test_insert(1, {x=3, y="five"})
    end)

    it("strings", function()
      test_insert('hello', 1)
      test_insert('hello', "five")
      test_insert('hello', {x=3, y="five"})
    end)

    it("tables", function()
      test_insert({"zeebra", 'kingkong'}, 1)
      test_insert({"zeebra", 'kingkong'}, "five")
      test_insert({"zeebra", 'kingkong'}, {x=3, y="five"})
    end)
    cache.purge()
  end)
  describe('for weak storage should give the same output as stored for types and be deleted when delete is called', function()
    it("numbers", function()
      test_insert(1, 1)
      test_insert(1, "five")
      test_insert(1, {x=3, y="five"})
    end)

    it("strings", function()
      test_insert('hello', 1)
      test_insert('hello', "five")
      test_insert('hello', {x=3, y="five"})
    end)

    it("tables", function()
      test_insert({"zeebra", 'kingkong'}, 1)
      test_insert({"zeebra", 'kingkong'}, "five")
      test_insert({"zeebra", 'kingkong'}, {x=3, y="five"})
    end)
    cache.purge()
  end)
  describe('for weak storage should give the same output as stored for types and be deleted when delete is called', function()
    it("numbers", function()
      local key = 1
      cache.put(key, 1, true)
      key = nil
      collectgarbage()
      local val = cache.get(1)
      assert(val == nil, 'Value should be removed from cache on garbage collection')
    end)

    it("strings", function()
      local key = 'hello'
      cache.put(key, "five", true)
      key = nil
      collectgarbage()
      local val = cache.get('hello')
      assert(val == nil, 'Value should be removed from cache on garbage collection')
    end)

    it("tables", function()
      local key = {"zeebra", 'kingkong'}
      cache.put(key, 1, true)
      local key = nil
      collectgarbage()
      local val = cache.get({"zeebra", 'kingkong'})
      assert(val == nil, 'Value should be removed from cache on garbage collection')
      test_insert({"zeebra", 'kingkong'}, {x=3, y="five"})
    end)
    cache.purge()
  end)
end)


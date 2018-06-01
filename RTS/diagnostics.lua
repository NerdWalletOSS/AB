local cache = require 'lua/cache'
local JSON = require 'lua/JSON'

local diagnostics = {}

diagnostics.diagnostics = function()
  local tests = cache.get('tests')
  local diag = {}
  for k,v in pairs(tests) do
    diag[#diag + 1] = v
  end 
  -- return diag
end


return diagnostics.diagnostics

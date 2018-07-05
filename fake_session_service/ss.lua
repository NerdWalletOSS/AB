local pegasus = require 'pegasus'
local data = require 'fake_session_service/init'
local JSON = require 'lua/JSON'

local server = pegasus:new({port='8024'})

server:start(function (request, response)
  local res = {}
  for k,v in pairs(data) do
    local num = math.random(0, #v)
    res[k]=v[num]
  end
   response:addHeader('Content-Type', 'application/json'):write(JSON:encode(res))
end)


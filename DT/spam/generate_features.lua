local assertx = require 'lua/assertx'
local function generate_features(in_features)
  assert(type(in_features) == "table")
  local out_features = {}
  for k, v in pairs(in_features) do 
    local kidx = assertx(tonumber(k), k .. ' is not a valid feature.')
    out_features[kidx] = assert(tonumber(v))
  end
  return out_features
end
return generate_features

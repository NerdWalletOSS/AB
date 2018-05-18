local assertx = require 'assertx'

local postal_cd_to_vars = require 'postal_cd_to_vars'
local time_to_vars      = require 'time_to_vars'
local get_referrer_type_cd = require 'get_referrer_type_cd'

local function generate_features(in_features)
  local out_features = {}
  assertx(in_features['country'] == 'US', 
    'Country of origin is ', tostring(in_features['country']), ', not "US".')
  local rtc = assert(get_referrer_type_cd(in_features))
  local pc = assert(postal_cd_to_vars(in_features['postal_cd']))
  local t = assert(time_to_vars(in_features['timestamp'],
    in_features['timezone']))
  for k, v in pairs(pc) do out_features[k] = v end
  for k, v in pairs(t) do out_features[k] = v end
  out_features['referral_type_cd'] = rtc
  -- copy some from in to out 
  out_features['os_fmly_nm']      = in_features['os_fmly_nm']
  out_features['device_type']     = in_features['device_type']
  out_features['browser_fmly_nm'] = in_features['browser_fmly_nm']
  return out_features
end

return generate_features

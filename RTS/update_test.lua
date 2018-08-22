require 'lua/str'
local assertx    = require 'lua/assertx'
local ffi        = require 'lua/ab_ffi'
local consts     = require 'lua/ab_consts'
local JSON       = require 'lua/JSON'

local function update_test(
  test,
  args
  )
  test = ffi.cast("TEST_META_TYPE *", test)
  T = assert(JSON:decode(args))
  
  test[0].id = tonumber(T.id)
  --[[ should be done in Lua but done in C because of uint64_t issues
  test[0].external_id = tonumber(T.external_id)
  test[0].seed = tonumber(T.seed)
  --]]
  local num_devices = test[0].num_devices
  assert(num_devices > 0)
  local is_dev_specific = test[0].is_dev_specific
  assert( ( is_dev_specific == true ) or ( is_dev_specific == false ) )

  local has_filters
  if (type(T.has_filters) == "boolean") then
    if ( T.has_filters ) then 
      has_filters = 1
    else
      has_filters = 0
    end
  elseif (type(T.has_filters) == "number") then
    has_filters = T.has_filters
  else
    assert(nil)
  end
  assert( (has_filters == 0 ) or (has_filters == 1 ) )
  -- TODO P2 We still need to incorporate filters into the code

  if ( is_dev_specific ) then 
    assert(num_devices > 1 )
  end

  test[0].ramp = tonumber(T.ramp)
  assert(test[0].ramp >= 0)
  local Variants = assert(T.Variants)
  local NumVariants = #Variants
  assert(NumVariants == test[0].num_variants)
  local sum = 0
  local num_final = 0
  local final_variant_id  = -1
  local final_variant_idx = -1
  for k1, v1 in ipairs(Variants) do
    for k2, v2 in ipairs(Variants) do 
      if ( k1 ~= k2 ) then 
        if ( v1.url ) and ( v2.url ) and (#v1.url > 0) then 
          assert(v1.url ~= v2.url)
        end
        if ( v1.name ) and ( v2.name ) then 
          assert(v1.name ~= v2.name)
        end
      end
    end
  end
    
  for k, v in ipairs(Variants) do
    assert(#v.name > 0)
    assert(#v.name <= consts.AB_MAX_LEN_VARIANT_NAME)
    ffi.copy(test[0].variants[k-1].name, v.name)

    if ( ( v.url ) and ( #v.url > 0 ) ) then 
      assert(#v.url <= consts.AB_MAX_LEN_URL)
      ffi.copy(test[0].variants[k-1].url, v.url)
    else
      assert(T.TestType ~= "XYTest")
    end
    num_final = num_final + tonumber(v.is_final)
    if ( tonumber(v.is_final) == 1 ) then 
      final_variant_id  = tonumber(v.id)
      final_variant_idx = k-1
    end

    if ( ( v.custom_data ) and ( #v.custom_data > 0 ) ) then 
      assert(#v.custom_data <= consts.AB_MAX_LEN_URL)
      ffi.copy(test[0].variants[k-1].custom_data, v.custom_data)
    end

    local percentage = tonumber(v.percentage)
    test[0].variants[k-1].id         = tonumber(v.id)
    test[0].variants[k-1].percentage = percentage
    assert( ( percentage >= 0 ) and ( percentage <= 100 ) ) 
    sum = sum + percentage
  end
  assert(num_final <= 1, "num_final = " .. num_final)
  assert( (sum > 99.99 ) and ( sum < 100.01 ) ) 
  if ( T.State == "started" ) then
    assert(num_final == 0, "test = " .. T.name .. " #final = " .. num_final)
    assert(final_variant_idx < 0)
    assert(final_variant_id  <  0)
    if ( T.TestType == "XYTest" ) then 
      if ( num_devices == 1 ) then
        local bidx = 0
        for k, v in pairs(Variants) do
          num_bins_to_set = v.percentage / 100.0 * consts.AB_NUM_BINS
          for i = 1, num_bins_to_set do
            test[0].variant_per_bin[0][bidx] = k-1
            bidx = bidx + 1 
          end
        end
      else
        assert(T.DeviceCrossVariant)
        local num_set = {}
        for d = 1, num_devices do
          num_set[d] = 0
        end
        for k, v in pairs(T.DeviceCrossVariant) do 
          local device_id = v.device_id
          assert(( device_id >= 1 ) and ( device_id <= num_devices) )
          local variant_id = v.variant_id
          local percentage = v.percentage
          assert((percentage >= 0 ) and (percentage <= 100))
           -- find variant_idx corresponding to this
          local variant_idx = 0
          for v = 1, NumVariants do 
            if ( Variants[v].id == variant_id ) then 
              variant_id = v-1
              break
            end
          end
          assert(( variant_idx >= 0 )and( variant_idx < NumVariants))
          local num_to_set = percentage / 100.0 * consts.AB_NUM_BINS
          start = num_set[device_id]
          -- print(device_id, variant_id, start, num_to_set)
          for l = 1, num_to_set do 
            assert(start+l <= consts.AB_NUM_BINS)
            test[0].variant_per_bin[device_id-1][start+l-1] = variant_idx
          end
          num_set[device_id] = num_set[device_id] + num_to_set
        end
      end
    elseif ( T.TestType == "ABTest" ) then 
      assert(num_devices == 1)
      -- make sure that Control is first variant
      local Variants = T.Variants
      local control_idx = -1
      for k, v in pairs(Variants) do
        if ( string.lower(v.name) == "control" ) then
          control_idx = k; break
        end
      end
      assert( control_idx >= 1 )
      if ( control_idx ~= 1 ) then 
        local swap = Variants[control_idx]
        Variants[control_idx] = Variants[1]
        Variants[1] = swap
      end
      -- Give everything to Control or (non_zero variant) at the start
      local default = 0
      if ( Variants[1].percentage == 0 ) then
        for k, v in pairs(Variants) do 
          if ( v.percentage ~= 0 ) then
            default = k-1
          end
        end
      end
      for b = 1, consts.AB_NUM_BINS do
        test[0].variant_per_bin[0][b-1] = default
      end
      -- Now for the rest of the variants to grab their spots
      for k, v in pairs(Variants) do
        local perc = tonumber(v.percentage)
        if ( ( perc ~= 0 ) and ( k ~= 1 ) ) then 
          local num_bins_to_set = perc / 100.0 * consts.AB_NUM_BINS
          local num_bins_set = 0
          local bidx = k-1
          for l = 1, num_bins_to_set do
            if ( bidx > consts.AB_NUM_BINS ) then break end 
            test[0].variant_per_bin[0][bidx] = k-1
            bidx = bidx + NumVariants
            num_bins_set = num_bins_set + 1
          end
          -- you now need to steal some stuff from Control
          if ( num_bins_set < num_bins_to_set ) then
            if ( num_bins_set == num_bins_to_set ) then break end 
            local start = (k-1-1) * NumVariants
            local incr = (NumVariants-1) * NumVariants
            for bidx = start, consts.AB_NUM_BINS-1, incr do
              test[0].variant_per_bin[0][bidx] = k-1
            end
            num_bins_set = num_bins_set + 1
          end
        end
      end
    else
      assert(nil)
    end
  elseif ( T.State == "terminated" ) then
    assert(num_final == 1 )
    assert(final_variant_idx >= 0)
    assert(final_variant_idx < NumVariants)
    assert(final_variant_id  >  0)
    for d = 0, num_devices do 
      test[0].final_variant_idx[d] = final_variant_idx
      test[0].final_variant_id[d]  = final_variant_id 
    end
  else
    assert(nil, "Bad state " .. T.State)
  end
 
  return true
end
return update_test

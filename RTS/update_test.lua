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

  test[0].has_filters = tonumber(T.has_filters)
  assert(( T.has_filters >= 0) and ( T.has_filters <= 1), args)

  test[0].ramp = tonumber(T.ramp)
  assert(test[0].ramp >= 0)
  local V = assert(T.Variants)
  assert(#V == test[0].num_variants)
  local sum = 0
  local num_final = 0
  local final_variant_id  = -1
  local final_variant_idx = -1
  for k1, v1 in ipairs(V) do
    for k2, v2 in ipairs(V) do 
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
    
  for k, v in ipairs(V) do
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
      final_variant_idx = k
    end

    if ( ( v.custom_data ) and ( #v.custom_data > 0 ) ) then 
      assert(#v.custom_data <= consts.AB_MAX_LEN_URL)
      ffi.copy(test[0].variants[k-1].custom_data, v.custom_data)
    end

    test[0].variants[k-1].id         = tonumber(v.id)
    test[0].variants[k-1].percentage = tonumber(v.percentage)
    assert( ( v.percentage >= 0 ) and ( v.percentage <= 100 ) ) 
    sum = sum + v.percentage
  end
  assert(num_final <= 1, "num_final = " .. num_final)
  assert( (sum > 99.99 ) and ( sum < 100.01 ) ) 
  if ( T.State == "started" ) then
    assert(num_final == 0 )
    assert(final_variant_idx < 0)
    assert(final_variant_id  <  0)
    if ( T.TestType == "XYTest" ) then 
      for d = 1, num_devices do 
        for b = 1, consts.AB_NUM_BINS do 
          test[0].variant_per_bin[d-1][b-1] = 0 -- TODO P1 
        end
      end
    elseif ( T.TestType == "ABTest" ) then 
      assert(num_devices == 1)
      -- make sure that Control is first variant
      local Variants = T.Variants
      for k, v in pairs(Variants) do
        if ( v.name == string.lower("Control") ) then
          control_idx = k; break
        end
      end
      if ( control_idx ~= 1 ) then 
        local swap = Variants[control_idx]
        Variants[control_idx] = Variants[1]
        Variants[1] = swap
      end
      -- Give everything to Control at the start
      for b = 1, consts.AB_NUM_BINS do
        test[0].variant_per_bin[0][b-1] = 0
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
            bidx = bidx + num_variants
            num_bins_set = num_bins_set + 1
          end
          -- you now need to steal some stuff from Control
          if ( num_bins_set < num_bins_to_set ) then
            if ( num_bins_set == num_bins_to_set ) then break end 
            local start = (k-1-1) * num_variants
            local incr = (num_variants-1) * num_variants
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

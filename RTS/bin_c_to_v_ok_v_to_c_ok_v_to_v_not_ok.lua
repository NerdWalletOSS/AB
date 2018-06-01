-- local dbg = require 'debugger'
local assertx = require 'lua/assertx'
local ffi = require 'lua/ab_ffi'
local consts = require 'lua/ab_consts'
local bin_c_to_v_ok_v_to_c_ok_v_to_v_not_ok = {}
local function set_variants_per_bin(c_test, variant_count)
  -- No device specific routing here
  --clean all the entries
  -- c_test.variant_per_bin = ffi.cast("uint8_t**", ffi.gc(
  -- ffi.C.malloc(ffi.sizeof("uint8_t*")), ffi.C.free))

  -- c_test.variant_per_bin[0] = ffi.cast("uint8_t*", ffi.gc(
  -- ffi.C.malloc(ffi.sizeof("uint8_t")*consts.AB_NUM_BINS), ffi.C.free))
  local variants = c_test.variants
  local variants_bin = c_test.variant_per_bin[0]
  ffi.fill(variants_bin, ffi.sizeof("uint8_t")*consts.AB_NUM_BINS)

  -- First use dedicated bins for each variant
  -- Dedicated bins for variant i are i, i+1*nV, i+2*nV ...
  for index = 1,variant_count -1 do -- Dont care about 0 index as that is control
    local variant = variants[index]
    local total_bins = math.floor(variant.percentage * 0.01 * consts.AB_NUM_BINS)
    local num_set = 0
    for set_index=index,consts.AB_NUM_BINS-1 ,variant_count do
      -- If target is reached then stop else claim the bin
      if num_set ~= total_bins then
        num_set = num_set + 1
        variants_bin[set_index] = index -- set_indexth bucket points to indexth variant
      end
    end
    if num_set ~= total_bins then
      -- If we dont have adequate bins we get buckets from control
      -- A variant can only get a bucket from control if (j/nV)%(i-1) == 0
      -- So, if nV = 3,
      -- Control's bins are 0, 3, 6, 9, 12, 15, ... These are
      -- renumbered by dividing by 3:  0, 1, 2, 3, 4,  5,
      --Variant 1 gets renumbered bins 0, 2, 4, ....
      --Variant 1 gets original   bins 0, 6, 12, ....
      --Variant 2 gets renumbered bins 1, 3, 5, ...
      --Variant 2 gets original   bins 3, 9, 15, ...
      local start = (index-1)*variant_count
      local incr = (variant_count-1)*variant_count
      for set_index=start, consts.AB_NUM_BINS -1 , incr do
        if num_set ~= total_bins then
          variants_bin[set_index] = ffi.cast("uint8_t", index)
          num_set = num_set + 1
        end
      end
    end
    assert(total_bins == num_set, "We should get the total number")
  end
end

function bin_c_to_v_ok_v_to_c_ok_v_to_v_not_ok.add_bins_and_variants(c_test, test_data)
  local is_dev_spec = assert(tonumber(test_data.is_dev_specific), "Must have boolean device specific or not")
  assert(is_dev_spec == consts.FALSE, "ABTests cannot have device specific routing")
  c_test.is_dev_specific = consts.FALSE

  local variants = assert(test_data.Variants, "Test should have variants")
  assert(type(variants) == "table", "Variants should be an array of variants")
  assertx(#variants >= consts.AB_MIN_NUM_VARIANTS and #variants <=consts.AB_MAX_NUM_VARIANTS,
  "Expected variants to be between ", consts.AB_MIN_NUM_VARIANTS, " and ", consts.AB_MAX_NUM_VARIANTS)
  c_test.num_variants = #variants
  -- c_test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(
  -- ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * #variants), ffi.C.free)) -- ffi malloc array of variants

  local pos, curr_index, total = 1, 0, 0
  -- sort the variants table
  -- ensures that control is at the first position
  table.sort(variants, function(a,b) return a.id < b.id end)
  if test_data.TestType == "ABTest" then
    assert(variants[1].name:lower() == "control", "First entry has to be control")
  end
  local final_variant_idx, final_variant_id
  for index, value in ipairs(variants) do
    entry = c_test.variants[index - 1]
    entry.id = assert(tonumber(value.id), "Expected to have entry for id of variant")
    if value.is_final ~= nil and tonumber(value.is_final) == consts.TRUE then
      final_variant_idx = index - 1
      final_variant_id = entry.id
    end
    entry.percentage = assert(tonumber(value.percentage), "Every variant must have a percentage")
    total = total + entry.percentage
    assertx(value.name and #value.name<= consts.AB_MAX_LEN_VARIANT_NAME, "Valid name for variant at position " , index)
    ffi.copy(entry.name, value.name)
    if value.url ~= nil then
      assert(#value.url >=0 and #value.url <= consts.AB_MAX_LEN_URL, "URL must have a valid length")
      -- dbg()
       assert(entry.url ~= nil, "Space must have been allocated for url")
       ffi.copy(entry.url, value.url)
    end
    -- TODO check lenfth of custom data
    assert(entry.custom_data ~= nil, "Space needs to be allocated for custom data")
    if value.custom_data ~= nil then
      ffi.copy(entry.custom_data, value.custom_data)
    else
      ffi.copy(entry.custom_data,"NULL")
    end
  end

  assertx(total == 100, "all the percentages should add up to 100, added to ", total)
  local test_state = test_data.State:lower()
  if test_state == "started" then
    c_test.final_variant_idx = nil
    c_test.final_variant_id = nil
    set_variants_per_bin(c_test, #variants)
  elseif test_state == "terminated" then
    -- c_test.final_variant_idx = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
    -- c_test.final_variant_id = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
    c_test.final_variant_idx[0] = assert(final_variant_idx, "Final variant idx cannot be nil for a terminated test")
    c_test.final_variant_id[0] = assert(final_variant_id, "Final variant id cannot be nil for a terminated test")

  else
    error("Unrecognized test state " .. test_data.State)
  end
  c_test.num_devices = 1
end

return bin_c_to_v_ok_v_to_c_ok_v_to_v_not_ok

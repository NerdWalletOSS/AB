-- local dbg = require 'debugger'
local bin_anonymous = {}
local assertx = require 'lua/assertx'
local consts = require 'lua/ab_consts'
local ffi = require 'lua/ab_ffi'
local spooky_hash = require 'RTS/spooky_hash'

local function set_variants_per_bin(bin, dev_variants, var_id_to_index_map)

  local num_set = 0
  local total_percentage = 0
  -- In case the bins dont fill up
  ffi.fill(bin, consts.AB_NUM_BINS, var_id_to_index_map[dev_variants[1].id])

  for index, variant in ipairs(dev_variants) do
    local percent = assert(tonumber(variant.percentage), "Variant must have a valid percentage")
    local total_bins = math.floor(percent * 0.01 * consts.AB_NUM_BINS)
    -- print(string.format("Variant pos: %s gets %s buckets starting at pos %s",
    -- var_id_to_index_map[variant.id], total_bins, num_set))
    assert(num_set + total_bins <= consts.AB_NUM_BINS, "Total number of bins should be less that total bins")
    for set_vals=num_set, num_set + total_bins - 1 do
      bin[set_vals] = var_id_to_index_map[variant.id]
    end
    num_set = num_set + total_bins
    total_percentage = total_percentage + percent
  end
  assertx(num_set <= consts.AB_NUM_BINS, "More than max bins cannot be occupied, total:", total)
  assertx(total_percentage == 100, "Total percentage should add up to 100 percent", total_percentage)
end

local function populate_variants(c_test, variants)
  -- dbg()
  local var_id_to_index_map = {}
  local final_variant_idx, final_variant_id
  assert(#variants >= consts.AB_MIN_NUM_VARIANTS and #variants <= consts.AB_MAX_NUM_VARIANTS, "invalid number of variants")
  c_test.num_variants = #variants -- TODO check for device specific too`
  table.sort(variants, function(a,b) return tonumber(a.id) < tonumber(b.id) end)
  -- TODO removed as mallocs in C c_test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * #variants), ffi.C.free)) -- ffi malloc array of variants
  -- ffi.fill(c_test.variants, ffi.sizeof("VARIANT_REC_TYPE") * #variants)
  -- TODO final variants, HELP
  for index, value in ipairs(variants) do
    entry = c_test.variants[index - 1]
    var_id_to_index_map[value.id] = index - 1
    entry.id = assert(tonumber(value.id), "Expected to have entry for id of variant")
    -- TODO check about final variant
    if tonumber(value.is_final) == consts.TRUE then
      final_variant_idx = index -1
      final_variant_id = entry.id
    end
    entry.percentage = assert(tonumber(value.percentage), "Every variant must have a percentage")
    -- TODO THIS NEEDS TO BE UNDONE, Ramesh to tell if name is absent for device
    -- specific
    assertx(value.name and #value.name<= consts.AB_MAX_LEN_VARIANT_NAME, "Invalid name for variant at position " , index, " NAME:", value.name)
    ffi.copy(entry.name, value.name)
    if c_test.test_type == "ABTest" then
      -- TODO ABTests cannpt ne anonymous
      assert("ABTest cannot be anonymous")
      -- entry.url = assertx(value.url, "Entry should have a url", " ID:", entry.id) -- TODO why do we have a max length?
    else
      local url = value.url
      assert(#url >=0 and #url <= consts.AB_MAX_LEN_URL, "URL must have a valid length")
      assert(entry.url ~= nil, "Space must have been allocated for url")
      ffi.copy(entry.url, url)
    end
    -- RODO make checks 
    assert(entry.custom_data ~= nil, "Space needs to be allocated for custom data")
    if value.custom_data ~= nil then
      ffi.copy(entry.custom_data, value.custom_data)
    else
      ffi.copy(entry.custom_data,"NULL")
    end

  end
  return var_id_to_index_map, final_variant_idx, final_variant_id
end

local function add_device_specific_terminated(c_test, test_data)
  local variants = test_data.Variants
  local num_devices = 0
  for _ in pairs(test_data.DeviceCrossVariant) do num_devices = num_devices + 1 end -- TODO this comes as a constant not based on count heret
  local var_id_to_index_map, final_variant_idx, final_variant_id = populate_variants(c_test, variants)
  c_test.variant_per_bin = nil
  -- TODO removed as mallocs in C c_test.final_variant_idx = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")*num_devices), ffi.C.free))
  -- TODO removed as mallocs in C c_test.final_variant_id = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")*num_devices), ffi.C.free))
  -- TODO right now we only have one final variant over time one final for each
  -- device
  for index=0,num_devices-1 do
    c_test.final_variant_idx[index] = final_variant_idx
    c_test.final_variant_id[index] = final_variant_id
  end

end

local function add_device_specific_started(c_test, test_data)
  -- get all variants
  local variants = test_data.Variants
  local num_devices = 0
  for _ in pairs(test_data.DeviceCrossVariant) do num_devices = num_devices + 1 end

  local var_id_to_index_map = populate_variants(c_test, variants)

  -- c_test.variant_per_bin = ffi.cast("uint8_t**", ffi.gc(
  -- TODO removed as mallocs in C ffi.C.malloc(ffi.sizeof("uint8_t*")*num_devices), ffi.C.free))
  -- ffi.fill(c_test.variant_per_bin, ffi.sizeof("uint8_t*")*num_devices)

  for index=0, num_devices-1 do
    -- c_test.variant_per_bin[index] = ffi.cast("uint8_t*", ffi.gc(
    -- TODO removed as mallocs in C ffi.C.malloc(ffi.sizeof("uint8_t")*consts.AB_NUM_BINS), ffi.C.free))
    -- ffi.fill(c_test.variant_per_bin[index] , ffi.sizeof("uint8_t")*consts.AB_NUM_BINS)
  end
  table.sort(test_data.DeviceCrossVariant, function(a,b) return tonumber(a[1].device_id) < tonumber(b[1].device_id) end)

  local index_to_key_map = {}
  for device_name, dev_variants in pairs(test_data.DeviceCrossVariant) do
    index_to_key_map[device_name] = dev_variants[1].device_id
  end
  for device_name, dev_variants in pairs(test_data.DeviceCrossVariant) do
    -- Also note that the id field in device specific part is useless and what
    -- we want is that variant_id so we can think of providing a getter function
    -- later but right now i am simply setting the id field to variant_id field

    for _,dev in ipairs(dev_variants) do
      dev.id = dev.variant_id
    end
    -- TODO strong assumption is that the devices will have ids starting from 0
    -- Idea is bin[device_id]
    local index = index_to_key_map[device_name] - 1

    -- print("Processing device:", device_name, "at index", index)
    set_variants_per_bin(c_test.variant_per_bin[index], dev_variants, var_id_to_index_map)
  end
  c_test.final_variant_idx = nil
  c_test.final_variant_id = nil
end

local function add_device_agnostic(c_test, test_data)
  local variants = test_data.Variants
  local var_id_to_index_map, final_variant_idx, final_variant_id = populate_variants(c_test, variants)
  local state = test_data.State:lower()
  if state == "started" then
    c_test.final_variant_idx = nil
    c_test.final_variant_id = nil
  elseif state == "terminated" then
    -- TODO removed as mallocs in C c_test.final_variant_idx = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
    -- TODO removed as mallocs in C c_test.final_variant_id = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
    c_test.final_variant_idx[0] = assert(final_variant_idx, "Final variant idx cannot be nil for a terminated test")
    c_test.final_variant_id[0] = assert(final_variant_id, "Final variant id cannot be nil for a terminated test")
  else
    error("Unknown test state " .. state)
  end
end

local function add_device_specific(c_test, test_data)
  local state = test_data.State:lower()
  if state == "started" then
    add_device_specific_started(c_test, test_data)
  elseif state == "terminated" then
    add_device_specific_terminated(c_test, test_data)
  else
    error("Unrecognized state " .. state)
  end
end

function bin_anonymous.add_bins_and_variants(c_test, test_data)
  local external_id = assert(test_data.external_id, "External id needs to be specified for test")
  c_test.external_id = spooky_hash.convert_str_to_u64(external_id)
  local is_dev_spec = assert(tonumber(test_data.is_dev_specific), "Must have boolean device specific or not")
  assert(is_dev_spec == consts.FALSE or is_dev_spec == consts.TRUE, "Test canonly have TRUE or FALSE in  device specific routing")
  c_test.is_dev_specific = is_dev_spec
  if tonumber(test_data.is_dev_specific) == consts.TRUE then
    add_device_specific(c_test, test_data)
  else
    add_device_agnostic(c_test, test_data)
  end
end

return bin_anonymous

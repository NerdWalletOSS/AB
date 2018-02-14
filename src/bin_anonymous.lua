local bin_anonymous = {}
local assertx = require 'assertx'
local ffi = require 'ab_ffi'
local consts = require 'ab_consts'

local function set_bins_for_device(variants_bin, variants)
  local function compare(a,b)
    return a.id < b.id
  end
  table.sort(variants, compare) -- TODO sorting should happen outside here is just creating the bins
  -- if test_type == "ABTest" then
  --   assert(ffi.string(variants[0].name):lower() == "control", "The first entry should always be control")
  -- end
  -- TODO expand to per device later
  local c_type = string.format("uint8_t[%s]", consts.AB_NUM_BINS)
  ffi.fill(variants_bin, ffi.sizeof(c_type))

  -- First use dedicated bins for each variant
  -- Dedicated bins for variant i are i, i+1*nV, i+2*nV ...
  for index = 1,#variants -1 do -- Dont care about 0 index as that is control
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
  end


end

local function add_device_specific(c_test, json_table)
  local variants = assert(json_table.Variants, "Test should have device specific variants")
  assert(type(variants) == "table", "Variants should be an array of variants")
  local compare = function(a,b)
    return a.device_id < b.device_id
  end
  for k,v in ipairs(variants) do -- sorting all the device specific variants
    table.sort(v)
  end

  c_test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(
  ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * #variants), ffi.C.free)) -- ffi malloc array of variants
  -- TODO this is not number of variants but number of devices that comes in a
  -- config
  -- c_test.final_variant_id = assert(json_table.FinalVariantID, "need a final variant") -- TODO check where this comes in
  local pos, curr_index, total = 1, 0, 0
  -- sort the variants table
  -- ensures that control is at the first position
  local function compare(a,b)
    return a.id < b.id
  end
  table.sort(variants, compare)
  -- TODO if ABTest
  c_test.final_variant_id = ffi.cast("unit32_t*", ffi.gc(
  ffi.C.malloc(ffi.sizeof("uint32_t")*#variants), ffi.C.free))
  ffi.fill(c_test.final_variant_id, ffi.sizeof("uint32_t")*#variants)
  local cast_type = string.format("uint8_t *(&)[%s]", consts.AB_NUM_BINS)
  c_test.variant_per_bin = ffi.cast(cast_type, ffi.gc(
  ffi.C.malloc(ffi.sizeof(cast_type)*#variants), ffi.C.free))
  ffi.fill(c_test.variant_per_bin, ffi.sizeof(cast_type)*#variants)
  -- TODO think about merging device specific with variants, right now all
  -- transitions are allowed in device specific so just fill bins one at a time
  for key, entry in pairs(variants) do
    local index = entry[0].device_id -- TODO somehow map device name to index position
    local mod_entry = {}
    for entry_index, entry_value in ipairs(entry) do
      local test = {}
      test.id = entry_value.variante_id
      test.percentage = entry_value.percentage
      mod_entry[#mod_entry + 1] = test
    end
    -- sort and call the poulation of the index
  end
  for index, value in ipairs(variants) do
    entry = c_test.variants[index - 1]
    entry.id = assert(tonumber(value.id), "Expected to have entry for id of variant")
    if entry.id == c_test.final_variant_id then
      c_test.final_variant_idx = curr_index
    end
    entry.percentage = assert(tonumber(value.percentage), "Every variant must have a percentage")
    total = total + entry.percentage
    -- print(curr_index, value.name)
    assertx(value.name and #value.name<= consts.AB_MAX_LEN_VARIANT_NAME, "Valid name for variant at position " , index)
    ffi.copy(entry.name, value.name)
    entry.url = assert(value.url, "Entry should have a url") -- TODO why do we have a max length?
    entry.custom_data = value.custom_data or "NULL" -- TODO why do we have a max length


  end
  assertx(total == 100, "all the percentages should add up to 100, added to ", total)
  set_variants_per_bin(c_test, #variants)
end


function bin_anonymous.add_bins_and_variants(c_test, test_data)
    local external_id = assert(test_data.external_id, "External id needs to be specified for test")
    c_test.external_id = spooky_hash.convert_str_to_u64(external_id)


end

return bin_anonymous

local ffi = require 'ffi'
-- TODO Set LD_LIBRARY_PATH to directory where .so is created
lupa_dt = ffi.load('liblupa_dt')
require 'DT/init'
local load_model = require 'DT/load_model'
local cdef_list = require 'lupa/cdef_list'
local release = require 'lupa/release'

local function init(model_dir, forest_type)
  conf = {} -- this is a global
  conf.DECISION_TREE_DIRECTORY = model_dir
  if ( not forest_type ) then 
    forest_type = "random_forest"
  end
  forest_type = string.lower(forest_type)
  assert(( forest_type == "random_forest")or(forest_type == "xgboost"))
  conf.FOREST_TYPE = forest_type
  --================================================
  assert(load_model(model_dir))
  --  cdef necessary structures
  local tbl = {}
  for _, h_file in ipairs(cdef_list) do
    for line in io.lines(h_file) do
      local fc = string.sub(line, 1, 1)
      if ( fc ~= '#' ) then 
        tbl[#tbl+1] = line
      end
    end
  end

  tbl[#tbl+1] = "void * malloc(size_t size); "
  tbl[#tbl+1] = " void *memset(void *s, int c, size_t n);"
  tbl[#tbl+1] = " void free(void *ptr); "
  ffi.cdef(table.concat(tbl, "\n"))
  --===========================================
  -- g_interp is a global
  -- g_interp = ffi.gc(ffi.C.malloc(1 * ffi.sizeof("DT_INTERPRETER_TYPE")), lupa_dt.free_interp)
  -- g_interp = ffi.gc(ffi.C.malloc(1 * ffi.sizeof("DT_INTERPRETER_TYPE")), release)
  g_interp = ffi.C.malloc(1 * ffi.sizeof("DT_INTERPRETER_TYPE"))
  ffi.fill(g_interp, ffi.sizeof("DT_INTERPRETER_TYPE"))
  g_interp = ffi.cast("DT_INTERPRETER_TYPE *", g_interp)
  local num_features = get_num_features()
  status = lupa_dt.load_models(model_dir, forest_type, num_features, g_interp)
  assert(status)

  return true
end
return init

-- package.path=package.path .. ";./../src/?.lua"
-- require 'strict'
local assertx = require 'lua/assertx'
local ffi = require 'lua/ab_ffi'
local JSON = require 'lua/JSON'
local consts = require 'lua/ab_consts'
local load_config = require 'RTS/ab_load_config'
local cache = require 'lua/cache'
-- local dbg = require 'debugger'

local config = "../src/ab.conf.json"

describe('Load config should', function()
  local confs_size = consts.NUM_SERVICES
  local g_conf = ffi.cast("CFG_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("CFG_TYPE")), ffi.C.free))
  local has_changed = ffi.cast("unsigned char*", ffi.gc(ffi.C.malloc(ffi.sizeof("unsigned char")* confs_size), ffi.C.free))
  ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
  ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )

  describe("check for validity of all fields", function()
  end)
  describe("load for a valid config", function()
    it("everything should have has_changed as true", function()
      ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
      ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
      -- dbg()
      load_config.load_config(config, g_conf, has_changed)
      for index=0, confs_size -1 do
        assertx(has_changed[index] == consts.TRUE, "Config should changed for index ", index)
      end
      ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
      ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
    end)
    it("should not have has_changed as true for unchanged elements", function()
      ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
      ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
      load_config.load_config(config, g_conf, has_changed)
      load_config.load_config(config, g_conf, has_changed)
      for index=0, confs_size -1 do
        assertx(has_changed[index] == consts.FALSE, "Config should be false for index ", index)
      end
      ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
      ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
    end)
    it("cache should be updated", function()
      cache.purge()
      load_config.load_config(config, g_conf, has_changed)
      ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
      ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
      local devices = cache.get("devices")
      assert(devices ~= nil and #devices > 0, "The cache should consist of devices")
      cache.purge()
    end)
    -- No longer needs as load is from file only
    -- it("should load same config from file too", function()
    --   local fname = "/tmp/test.json"
    --   local file = io.open(fname, "w")
    --   file:write(config)
    --   file:close()
    --   local status, res = pcall(load_config.load_config_from_file, g_conf, has_changed, fname)
    --   assert(status == true, res)
    --   os.remove(fname)
    -- end)
    -- it("", function()
    -- end)
    -- it("should fail for an invalid db", function()
    -- end)
    -- it("should update devices", function()
    -- end)
    -- it("should say has_changed when values changed only", function()
    -- end)
    --
  end)
end)

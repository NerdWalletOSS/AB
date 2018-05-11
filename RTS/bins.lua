local bins = {}
bins.anonymous = require 'RTS/bin_anonymous'
-- bins.free_for_all = require 'bin_free_for_all'
-- bins.memory_c_to_v_ok = require 'memory_c_to_v_ok'
-- bins.memory_no_changes = require 'memory_no_changes'
bins.c_to_v_ok_v_to_c_ok_v_to_v_not_ok = require 'bin_c_to_v_ok_v_to_c_ok_v_to_v_not_ok'

return bins

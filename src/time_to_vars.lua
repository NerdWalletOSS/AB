local luatz = require 'luatz.init'
local assertx = require 'assertx'
local MAX_TS = 2147483648
local NY_2018 = 1514764800

local function time_to_vars(t, tz)
	--[[
	This takes either a (string or numeric) timestamp or a RFC 3339 string (as time)
	and a string timezone, and return whatever variables is involved in the transform.

	Params:
	-- t: a string denoting a timestamp, a RFC 3339 string denoting a time, or a number (timestamp).
		- example of RFC 3339: '2018-03-01T20:23:45Z'
	-- tz: a string denoting a timezone, e.g. 'America/Los_Angeles'

	Will fail if:
	-- tz is not a valid time zone
	-- t is not a valid string or number
		-- t can be a number, but if it is out of range it will fail. There is a max/min timestamp range.
	-- 
	]]
	local status, tz_value = pcall(luatz.get_tz, tz) -- tz_value is a table
	assertx(status, 'Invalid time zone string: ', tz, ' is not a valid time zone string.')
	if type(t) == 'string' then
		if not tonumber(t) then
			local status, tt = pcall(luatz.parse.rfc_3339, t) -- tt is a table
			assertx(status and tt, 'Invalid time string: ', t, ' is not a valid time string.')
			t = tt:timestamp() -- t is a timestamp, i.e. number
		else
			t = tonumber(t) -- if it was a '122342324' string
		end
	end
	assertx(t <= MAX_TS and t >= 0, 'Timestamp ', t, ' is not in range:  [', 0, ', ', MAX_TS, ']')
	-- now, factor in the time zone diff
	local local_t = tz_value:localise(t) -- local_t is a timestamp
	local local_tt = luatz.timetable.new_from_timestamp(local_t) -- local_tt is a table
	local utc_offset = tz_value:find_current(NY_2018).gmtoff / 3600 
	-- that time stamp represents time at new year 2018 GMT. 
	-- that step gets the winter offset of relevant time zone, therefore
	-- tz_off is a (most likely negative) number, ideally either of
	-- -5.0, -6.0, -7.0, -8.0, -9.0, -10.0
	local day_of_wk = local_tt.wday - 2 -- the plus 2 is to match python's convention of wday (mon starts on 0 there)
	if day_of_wk < 0 then
		day_of_wk = day_of_wk + 7
	end
	local local_hour = local_tt.hour
	local is_working = 0 <= day_of_wk and day_of_wk <= 4 and 9 <= local_hour and local_hour <= 18
	local is_late_at_night = 1 <= local_hour and local_hour < 7
	return {
		year = local_tt.year,
		month = local_tt.month,
		day_of_wk = day_of_wk,
		local_hour = local_tt.hour,
		is_working = is_working,
		is_late_at_night = is_late_at_night,
		utc_offset = tostring(utc_offset)
	}
end

return time_to_vars
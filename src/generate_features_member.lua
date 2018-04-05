--[[XVARS = [u'cs', u'otp', u'util', u'q', u'ty',
       u'o', u'derog', u'open_mort', u'oage',
       u'aage', u'cl',
       u'pastdue', u'lastpaid',
       u'pmtfrac', u'late_30', u'late_60', u'late_90']]--
local assertx = require 'assertx'
local luatz = require 'luatz.init'
local MAX_TS = 2147483648
local open_mort_codes = {['SM']=true, ['CV']=true, ['FR']=true, ['VM']=true, ['FL']=true, ['RM']=true, ['CMG']=true}


local function validate_input(args)
	local var = assert(args['var'], "Variable is missing. Supply 'var'.")
	var = assertx(tonumber(var), "Variable is not a number, but a(n) ", tostring(type(var)))
	local var_nm = assert(args['var_nm'], "Variable name is missing. Supply 'var_nm'.")
	local lb = args['lb'] or math.mininteger
	local ub = args['ub'] or math.maxinteger
	assertx(var >= lb and var <= ub, "Variable ", tostring(var_nm), " with value ", var, " is out of range, which is [", lb, ", ", ub, "].")
	return var
end


local function generate_tradelines_features(tradelines, ts) 
	-- verify above that time will be verified before putting in here
	-- Each tradeline is a dictionary; we are interested in many things.
	local q, ty, o, derog, oage, aage, cl, pastdue, lastpaid, pmtfrac, late_30, late_60, late_90 = 0, 0, 0, 0, 0, 0, 0, 0, math.maxinteger, 0, 0, 0, 0
	local sum_age, mly_pmt, sum_past_due = 0, 0, 0
	local open_mort = false
	for _, tl in pairs(tradelines) do
		assert(tl['open_closed_symbol'], 'Open-closed symbol not present.')
		assert(tl['account_subtype'], 'Account subtype not present.')
		if tl['account_subtype'] == 'Collection Account' then
			tl['late_30_count'] = 0 -- this is because collection accounts return a NA/i.e. it's not present in the JSON blob.
			tl['late_60_count'] = 0
			tl['late_90_count'] = 0
		end
		if tl['open_closed_symbol'] == 'O' then
			local date_opened = assert(tl['date_opened_at'], 'Date opened at is not present.')
			local status, tt = pcall(luatz.parse.rfc_3339, date_opened) -- tt is a table
			assertx(status and tt, 'Invalid time string: ', date_opened, ' is not a valid time string. Error: ', tt)
			local days_open = math.floor((ts - tt:timestamp()) / (86400)) -- ts should be a timestamp, floor because concerned only with whole number days
			assertx(days_open >= 0, 'days_open timestamp, ', tostring(tt:timestamp()), 'is geq than click timestamp, ', ts)
			if days_open <= 90 then q = q + 1 end
			if days_open <= 730 then ty = ty + 1 end
			if not open_mort and open_mort_codes[tl["account_subtype_symbol"]] ~= nil then open_mort = true end
			if days_open > oage then oage = days_open end
			sum_age = sum_age + days_open
			o = o + 1
			local credit_limit = validate_input({['var']=tl['credit_limit'], ['var_nm']='credit_limit', ['lb']=0})
			local amount_past_due = validate_input({['var']=tl['amount_past_due'], ['var_nm']='amount_past_due', ['lb']=0})
			cl = cl + credit_limit
			sum_past_due = sum_past_due + amount_past_due
			local last_payment_date = assert(tl['last_payment_date'], 'Last payment date is not present.')
			local status, tt_payment = pcall(luatz.parse.rfc_3339, last_payment_date) -- tt is a table
			assertx(status and tt_payment, 'Invalid time string: ', last_payment_date, ' is not a valid time string. Error: ', tt_payment)
			local lastpaid_days = math.floor((ts - tt_payment:timestamp()) / (86400)) -- ts should be a timestamp, floor because concerned only with whole number days
			assertx(lastpaid_days >= 0, 'lastpaid timestamp, ', tostring(tt_payment:timestamp()), 'is geq than click timestamp, ', ts)
			if lastpaid_days < lastpaid then lastpaid = lastpaid_days end
			if credit_limit > 0 then
				local monthly_payment = validate_input({['var']=tl['monthly_payment'], ['var_nm']='monthly_payment', ['lb']=0})
				mly_pmt = mly_pmt + monthly_payment
			end
		end
		assert(tl['account_type_symbol'], 'Account type symbol not present.')
		if tl['account_type_symbol'] == 'F' then derog = derog + 1 end

		late_30_ct = validate_input({['var']=tl['late_30_count'], ['var_nm']='late_30', ['lb']=0})
		late_60_ct = validate_input({['var']=tl['late_60_count'], ['var_nm']='late_60', ['lb']=0})
		late_90_ct = validate_input({['var']=tl['late_90_count'], ['var_nm']='late_90', ['lb']=0})
		late_30 = late_30 + late_30_ct
		late_60 = late_60 + late_60_ct
		late_90 = late_90 + late_90_ct
	end
	if o > 0 then aage = sum_age / o end
	if cl > 0 then
		pastdue = sum_past_due / cl
		pmtfrac = mly_pmt / cl
	end
	if lastpaid == math.maxinteger then lastpaid = 0 end
	return {
		['q']=q,
		['ty']=ty,
		['o']=o,
		['derog']=derog,
		['open_mort']=open_mort,
		['oage']=oage,
		['aage']=aage,
		['cl']=cl,
		['pastdue']=pastdue,
		['lastpaid']=lastpaid,
		['pmtfrac']=pmtfrac,
		['late_30']=late_30,
		['late_60']=late_60,
		['late_90']=late_90}
end


local function generate_features(in_features)
	local cs = validate_input({['var']=in_features['vantage_score_3'], ['var_nm']='cs', ['lb']=300, ['ub']=850})
	local otp = validate_input({['var']=in_features['percent_on_time_payments'], ['var_nm']='otp', ['lb']=0.0, ['ub']=1.0})
	local util = validate_input({['var']=in_features['percent_utilization'], ['var_nm']='util', ['lb']=0.0})
	local tradelines = assert(in_features['tradelines'], "Tradeline non-existent.")
	local ts = assert(in_features["timestamp"], "Timestamp non-existent.")
	assertx(type(ts) == 'number' or (type(ts) == 'string' and tonumber(ts)), 'Timestamp not a correct format, format: ', type(ts))
	if type(ts) == 'string' then ts = tonumber(ts) end
	assertx(ts <= MAX_TS and ts >= 0, 'Timestamp ', ts, ' is not in range:  [', 0, ', ', MAX_TS, ']')
	local out_features = assert(generate_tradelines_features(tradelines, ts), "features could not be generated.")
	out_features['cs'] = cs
	out_features['otp'] = otp
	out_features['util'] = util
	return out_features
end


return generate_features
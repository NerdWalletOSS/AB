local cache = require 'cache'

local function get_referrer_type_cd(args)
	local utm_med = args['utm_med'] or ''
	local utm_src = args['utm_src'] or ''
	local utm_camp = args['utm_camp'] or ''
	local host = args['host'] or ''
	local domain = args['domain'] or ''
	domain = string.gmatch(domain, '(.-)%.')() -- gets back the first host, e.g. google.com -> google
	local table_isn = assert(cache.get("table_isn"), "Cache missing table_isn.")
	local table_mvc = assert(cache.get("table_mvc"), "Cache missing table_mvc.")
	local table_rd_sm = assert(cache.get("table_rd_sm"), "Cache missing table_rd_sm.")
	local table_rd_search = assert(cache.get("table_rd_search"), "Cache missing table_rd_search.")
	local ext_dw_referral_sk = 0
	if table_isn[domain] ~= nil then
		ext_dw_referral_sk = 2
	elseif domain == nil or domain == '' then
		ext_dw_referral_sk = 0
	elseif table_rd_sm[host] ~= nil then
		ext_dw_referral_sk = 1
	elseif table_rd_search[host] ~= nil then
		ext_dw_referral_sk = 3
	end

	local mvd_vendor_nm = nil -- for reasons unknown, if I remove this line, 50% of outputs don't match my original
	local mvc_chnl_nm = nil -- for reasons unknown, if I remove this line as well 50% of outputs don't match my original
	mvc_value = table_mvc[utm_src]
	if mvc_value ~= nil then
		mvd_vendor_nm = mvc_value['vendor_nm']
		mvc_chnl_nm = mvc_value[utm_med]
	end
	
	mvd_c = mvd_vendor_nm or domain
	mvd_c_check = {['twitter']=1, ['facebook']=2, ['t']=3, ['linkedin']=4, ['lnkd']=5, ['pinterest']=6}
	if mvd_c_check[mvd_c] or mvc_chnl_nm == 'social' or ext_dw_referral_sk == 1 then
		referral_type_cd = 'Social Media'
	elseif mvc_chnl_nm and mvc_chnl_nm ~= 'sem' and mvc_chnl_nm ~= 'social' then
		referral_type_cd = 'Referral'
	elseif ext_dw_referral_sk == 3 or mvc_chnl_nm == 'sem' then
        referral_type_cd = 'Search'
    elseif mvd_c == 'nerdwallet' or ext_dw_referral_sk == 2 then
        referral_type_cd = 'Internal'
    elseif (utm_camp and utm_camp ~= '') or mvc_value or (domain and domain ~= '') then
        referral_type_cd = 'Referral'
    else
    	referral_type_cd = 'Unknown'
    end
    return referral_type_cd
end

return get_referrer_type_cd

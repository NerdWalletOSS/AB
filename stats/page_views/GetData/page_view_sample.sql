drop table if exists dw_workarea.rst_page_view_sample
;

create table dw_workarea.rst_page_view_sample as
select dw_page_view_id
, view_dw_page_sk
, DATEDIFF(second,'2015-10-01 00:00:00.000', page_view_utc_ts) as diff_secs
, page_view_utc_ts
, dw_site_visitor_id as uuid 
from dw_page_view_event_f
where 1=1
and dw_suspected_bot_in = 'False'
and dw_site_visitor_id > 0
and dw_eff_dt >= '2015-10-01'
and dw_eff_dt < '2015-10-08'
order by 3
;

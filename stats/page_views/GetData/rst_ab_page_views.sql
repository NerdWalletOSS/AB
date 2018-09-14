/* TL1 */
drop table if exists rst_ab_tl1
;

create table rst_ab_tl1 as
select ab_test_id as test_id
, ab_ramp_id as ramp
, ab_variant_id as variant_id
, dw_site_visitor_id as user_id
, ab_log_event_utc_ts as entry_ts
from dw_ab_log_event_f 
where 1=1
and ab_test_id = 1
and ab_ramp_id = 1
and dw_site_visitor_id > 0
;

drop table if exists rst_ab_ramp_times
;

create table rst_ab_ramp_times as
select distinct start.ab_test_id as test_id
, start.chg_nr as ramp
, nvl(finish.chg_nr, start.chg_nr) as next_ramp
, start.start_time as start_time
, nvl(finish.end_time, sysdate) as end_time
from
(select ab_test_id
, chg_nr
, ab_ramp_chg_utc_ts as start_time
from dw_ab_ramp_d
where 1=1
and ab_test_id = 1
and chg_nr = 1
) start
left outer join
(select ab_test_id
, chg_nr
, ab_ramp_chg_utc_ts as end_time
from dw_ab_ramp_d
where 1=1
and ab_test_id = 1
and chg_nr = 1+1
) finish
on start.ab_test_id = finish.ab_test_id and start.chg_nr + 1 = finish.chg_nr
;

drop table if exists rst_ab_valid_user
;

create table rst_ab_valid_user as
select a.test_id
, a.ramp
, a.variant_id
, a.user_id
, a.entry_ts
, b.end_time
from rst_ab_tl1 a
join
rst_ab_ramp_times b
on a.test_id = b.test_id and a.ramp = b.ramp
and a.entry_ts >= b.start_time
and a.entry_ts <= b.end_time
;

drop table if exists rst_ab_user_page_view
;

create table rst_ab_user_page_view as
select v.test_id
, v.ramp
, v.variant_id 
, v.user_id
, p.pv_id
from
rst_ab_valid_user v
join
(select dw_page_view_id as pv_id
-- , view_dw_page_sk as pv_sk
, page_view_utc_ts as pv_ts
, dw_site_visitor_id as user_id 
from dw_page_view_event_f
where 1=1
and dw_suspected_bot_in = 'False'
and dw_site_visitor_id > 0 ) p
on v.user_id = p.user_id
and p.pv_ts >= v.entry_ts
and p.pv_ts <= v.end_time
;

drop table if exists rst_ab_user_pv_count
;

create table rst_ab_user_pv_count as
select test_id, ramp, variant_id, user_id, count(*) as num_pages
from rst_ab_user_page_view
group by test_id, ramp, variant_id, user_id
;

drop table if exists rst_ab_numpvs_user_count
;

create table rst_ab_numpvs_user_count as
select test_id, ramp, variant_id, num_pages, count(user_id) as num_users
from rst_ab_user_pv_count
group by test_id, ramp, variant_id, num_pages
;

drop table if exists rst_ab_desc_stats
;

create table rst_ab_desc_stats as
select test_id, ramp, variant_id
, count(user_id) as num_users
, avg(num_pages::float) as mean_pv
, stddev(num_pages)/sqrt(count(user_id)) as SE_mean_pv
from rst_ab_user_pv_count
group by test_id, ramp, variant_id
;

drop table if exists rst_ab_z_p_stats
;

create table rst_ab_z_p_stats as
select tst.*
, (tst.mean_pv -ctrl.mean_pv) as delta_pvs
, (tst.mean_pv -ctrl.mean_pv)/sqrt(tst.se_mean_pv^2 + ctrl.se_mean_pv^2) as z
, round(dm_data_science.f_calc_pval_from_z((tst.mean_pv -ctrl.mean_pv)/sqrt(tst.se_mean_pv^2 + ctrl.se_mean_pv^2)), 1) as p_value
from
rst_ab_desc_stats tst
join
(select * from rst_ab_desc_stats
where variant_id = 1) ctrl
on tst.test_id = ctrl.test_id
and tst.ramp = ctrl.ramp
;



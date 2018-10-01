drop table if exists rt_try1
;
create temp table rt_try1 as
select top 100 * from dw_page_view_enriched where dw_eff_dt > '2015-12-18'
;
unload ('select * from rt_try1')
to 's3://nerdwallet-dwh-redshift/rt_try1'
credentials 'aws_access_key_id=REDACTED;aws_secret_access_key=REDACTED'
delimiter as ','  -- optional, default is '|'
PARALLEL off    -- optional, default is stored in many files (~10), parallel off means the data will be in one file
;

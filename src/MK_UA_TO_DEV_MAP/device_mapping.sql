select case when device = 'Desktop' then 'Desktop'
            when device = 'Mobile' and os_type = 'Android' then 'Mobile_Android'
            when device = 'Mobile' and device_type in ('iPad','iPhone') then 'Mobile_iOS'
            when device = 'Tablet' and os_type = 'Android' then 'Tablet_Android'
            when device = 'Tablet' and device_type in ('iPad','iPhone') then 'Tablet_iOS'
            else 'Other' end justin_class, *
            
from (
    select 
             CASE
           WHEN c.is_mobile_in::text = 'True'::character varying::text THEN 'Mobile'::character varying
           WHEN c.is_tablet_in::text = 'True'::character varying::text THEN 'Tablet'::character varying
           ELSE 'Desktop'::character varying
       END AS device,
       CASE
           WHEN c.conf_device_fmly_nm::text = 'Tablet'::character varying::text
                AND c.conf_os_fmly_nm::text = 'Android'::character varying::text THEN 'Android Tablet'::character varying
           WHEN c.conf_device_fmly_nm::text = 'Windows'::character varying::text
                AND c.os_fmly_nm::text = 'Windows Phone'::character varying::text THEN 'Windows Phone'::character varying
           WHEN c.conf_device_fmly_nm::text = 'Windows'::character varying::text THEN 'PC'::character varying
           ELSE c.conf_device_fmly_nm
       END AS device_type,
       c.conf_os_fmly_nm AS os_type,
       c.conf_browser_fmly_nm AS browser_type, count(*)
    from dw_report.user_agent_d c 
    group by 1,2,3,4
) a
    ;

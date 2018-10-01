#!/bin/bash

/data/etl/Common/redshift_sql_command.sh ~/WORK/PageViews/GetData/rst_page_views.sql

/data/etl/Common//redshift_unload_command.sh ~/WORK//PageViews/GetData/page_view_z_p_unload.sql /rtate/ _rst_page_view_z_p_stats_

aws s3 cp 's3://nerdwallet-dwh-redshift/rtate/_rst_page_view_z_p_stats_000' ~/WORK/PageViews/Data/.

/data/etl/Common//redshift_unload_command.sh ~/WORK//PageViews/GetData/page_view_by_uuid_unload.sql /rtate/ _rst_page_view_by_uuid_

aws s3 cp 's3://nerdwallet-dwh-redshift/rtate/_rst_page_view_by_uuid_000' ~/WORK/PageViews/Data/.

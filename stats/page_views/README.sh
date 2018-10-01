#!/bin/bash

./Config/redshift_sql_command.sh ./GetData/rst_page_views.sql

./Config/redshift_unload_command.sh ./GetData/page_view_z_p_unload.sql /rtate/ _rst_page_view_z_p_stats_

aws s3 cp 's3://nerdwallet-dwh-redshift/rtate/_rst_page_view_z_p_stats_000' ./Data/.

./Config/redshift_unload_command.sh ./GetData/page_view_by_uuid_unload.sql /rtate/ _rst_page_view_by_uuid_

aws s3 cp 's3://nerdwallet-dwh-redshift/rtate/_rst_page_view_by_uuid_000' ./Data/.

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'insert_row.php'; // NO PROBLEM
require_once 'get_time_usec.php'; // NO PROBLEM
require_once 'lkp.php';

function start_log($str_inJ)
{
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "insert_test_edit_test_basic");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;
  $ret_val['created_at'] = $created_at;
  $ret_val['updated_at'] = $updated_at;
  $ret_val['t_create'] = $t_create;
  $ret_val['t_update'] = $t_update;
  $ret_val['api_id'] = $api_id;
  $ret_val['request_webapp_id'] = $request_webapp_id;
  return $ret_val;
}
?>

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
require_once 'dbconn.php';
require_once 'insert_row.php'; // NO PROBLEM
require_once 'get_time_usec.php'; // NO PROBLEM
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'mod_row.php';
require_once 'inform_rts.php';

function set_device_specific(
  $str_inJ
)
{
  //--- Logging 
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "insert_test_edit_test_basic");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;

  // START Check inputs
  assert(!empty($str_inJ));
  assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");
  $dxv = get_json_element($inJ, 'DeviceCrossVariant'); 
  rs_assert($dxv);
  foreach ( $dxv as $d => $v ) { 
    $device_id   = lkp($d, "device");
    rs_assert(is_array($v));
  }
  //------------------------------------------
  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["msg_stdout"] = "Set Device Specific Variants for [$test_name] ";
  $Y['msg_stdout']  = $outJ["msg_stdout"];
  $Y['status_code'] = $outJ["status_code"];
  db_set_row("log_ui_to_webapp", $request_webapp_id, $Y);
  // Note it is possible for both msg_stdout and msg_stderr to be set
  if ( $state == "started" ) {
    $status = inform_rts($test_id, $rts_err_msg);
    if ( !$status ) { 
      $http_code = 400; 
      $Y['msg_stderr'] = $rts_err_msg;
    }
  }
  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

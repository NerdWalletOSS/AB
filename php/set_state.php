<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
require_once 'dbconn.php';
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'db_set_row.php';
require_once 'inform_rts.php';

function start_test(
  $test_id
)
{
  //--- Logging 
  $created_at = $updated_at = get_date(); 
  $t_create   = get_time_usec(); 
  $api_id   = lkp("api", "start_test");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;
  //--------------------------------------
  // START Check inputs
  assert(!empty($str_inJ));
  assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");

  $test_id = get_json_element($inJ, 'id'); 
  $new_state  = get_json_element($inJ, 'NewState'); 

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");
  $state_id = $T['state_id'];
  $state   = lkp("state", $state_id, "reverse");

  $X1['updated_at'] = $updated_at;
  switch ( $action ) {
  case "dormant" : 
    rs_assert($state == "draft");
    $X1['state_id'] = lkp("state", "dormant");
    break;
  case "started" : 
    rs_assert($state == "dormant");
    $X1['state_id'] = lkp("state", "started");
    break;
  case "terminated" : 
    $winner  = get_json_element($inJ, 'Winner'); 
    $X1['state_id'] = lkp("state", "terminated");
    $winner_id = 0; // TODO verify that winner is a valid variant 
    $X2['is_final'] = 1;

    break;
  case "archived" : 
    break;
  default : 
    rs_assert(null, "Invalid new state [$new_state]");
  }
    // TODO Use transaction
  db_set_row("test", $test_id, $X1);
  if ( isset($X2 ) { 
    db_set_row("variant", $winner_id, $X2);
  }
  //---------------------------------

  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["msg_stdout"] = "Started Test $test_id ";
  db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
  // Note it is possible for both msg_stdout and msg_stderr to be set
  $status = inform_rts($test_id, $rts_err_msg);
  if ( !$status ) { 
    $http_code = 400; 
    $Y['msg_stderr'] = $rts_err_msg;
  }

  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

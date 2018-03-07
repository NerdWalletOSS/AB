<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
require_once 'dbconn.php';
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'db_set_row.php';
require_once 'inform_rts.php';
require_once 'chk_test_basic.php';

function set_state(
  $str_inJ
)
{
  //--- Logging 
  $created_at = $updated_at = get_date(); 
  $t_create   = get_time_usec(); 
  $api_id   = lkp("api", "set_state");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;
  //--------------------------------------
  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); rs_assert($inJ, "invalid JSON");

  $test_id = get_json_element($inJ, 'id'); 
  $attr    = get_json_element($inJ, 'Attribute'); 
  $values  = get_json_element($inJ, 'Values');
  $updater_id = lkp("admin", $updater);

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");
  $state_id = $T['state_id'];
  $state   = lkp("state", $state_id, "reverse");
  rs_assert( ( $state == "draft" ) || ( $state == "dormant" ) );
  $A = db_get_row("attr", "name", $attr);
  rs_assert($A, "attribute $attr not found");
  $attr_type_id = $A['attr_type_id'];
  $attr_type = lkp("attr_type", $attr_type_id, "reverse");
  switch ( $attr_type ) { 
  case "boolean" : 
    rs_assert(($values === true ) || ( $values === false ));
    $bval = $values;
    break;
  case "numeric" : 
    rs_assert(is_numeric($values));
    $nval = $values;
    break;
  case "categorical" : 
    rs_assert(is_array($values));
    foreach ( $values as $v ) { // verify values and convert to IDs
    }
    break;
  default : 
  rs_assert(false, "attribute type  $attr_type not found");

    break;
  }
  //--------------------------------------
  // Verify that test is in good state. Ideally, needed only for 
  // draft->dormant transition but never hurts to leave it in 
  $t1 = db_get_test($test_id);
  $t2 = json_decode(json_encode($t1));
  $chk_rslt = chk_test_basic($t2, true);
  rs_assert($chk_rslt);
  $X1['updated_at'] = $updated_at;
  $X1['updater_id'] = $updater_id;

  $r = db_get_row("bool_attr_test", "test_id", $test_id);
  if ( $r == null ) { 
    insert_row("bool_attr_test", $X);
  }
  else {
    mod_row("bool_attr_test", $X);
  }
  //-- STOP : Database updates
  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["msg_stdout"] = "Changed filter for Test $test_id ");
  db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

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
  $new_state  = get_json_element($inJ, 'NewState'); 
  $updater    = get_json_element($inJ, 'Updater');
  $updater_id = lkp("admin", $updater);

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");

  $state_id  = intval($T['state_id']);
  $old_state = lkp("state", $state_id, "reverse");

  $test_type_id = intval($T['test_type_id']);
  $test_type    = lkp("test_type", $test_type_id, "reverse");
  // return if new state is same as current one or old == archived
  if ( ( $old_state != $new_state ) && ( $old_state == "archived" ) ) { 
    $outJ["status_code"] = 400;
    $outJ["msg_stdout"] = "No change once test is archived\n";
    db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
    header("Error-Code: 400");
    return $outJ;
  }
  if ( ( $old_state == $new_state ) || ( $old_state == "archived" ) ) { 
    $outJ["status_code"] = 200;
    $outJ["msg_stdout"] = "No change in state for $test_id from $new_state";
    db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
    header("Error-Code: 200");
    header("ChangeState: false");
    http_response_code(200);
    return $outJ;
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
  $X1['request_webapp_id']  = $request_webapp_id;
  $X1['api_id']       = $api_id;
  switch ( $new_state ) {
  case "dormant" : 
    rs_assert($old_state == "draft");
    $X1['state_id'] = lkp("state", "dormant");
    break;
  case "started" : 
    rs_assert(
      ( ( $old_state == "dormant") || 
        ($old_state == "terminated" && ( $test_type == "XYTest") ) ), 
      "can start a test only if prevous state == dormant, not $old_state");
    $X1['state_id'] = lkp("state", "started");
    break;
  case "terminated" : 
    $winner  = trim(get_json_element($inJ, 'Winner'));
    rs_assert($winner, "Need to provide winner to terminate test");
    $X1['state_id'] = lkp("state", "terminated");
    $v = db_get_row("variant", "name", $winner, " and test_id = $test_id ");
    rs_assert($v);
    $losers = db_get_rows("variant", 
      "test_id = $test_id and name != '$winner' ");
    rs_assert($losers);
    $winner_id = $v['id'];
    $X2['is_final'] = 1;
    $X2['percentage'] = 100;
    $X2['request_webapp_id']  = $request_webapp_id;
    $X2['api_id']       = $api_id;


    break;
  case "archived" : 
    // any state can lead to archived
    $X1['state_id'] = lkp("state", "archived");
    break;
  default : 
    rs_assert(null, "Invalid new state [$new_state]");
  }
  //-- START: Database updates
  $dbh = dbconn(); rs_assert($dbh); 
  try {
    $dbh->beginTransaction();
    db_set_row("test", $test_id, $X1);
    if ( isset($X2) ) { 
      db_set_row("variant", $winner_id, $X2);
      $X3['percentage'] = 0;
      $X3['request_webapp_id']  = $request_webapp_id;
      $X3['api_id']     = $api_id;
      foreach ( $losers as $l ) {
        $loser_id = $l['id'];
        db_set_row("variant", $loser_id, $X3); 
      }
    }
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-- STOP : Database updates
  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["msg_stdout"] = "Changed state of Test $test_id from $old_state to $new_state";
  db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
  // Note it is possible for both msg_stdout and msg_stderr to be set
  $status = inform_rts($test_id, $rts_err_msg);
  if ( !$status ) { 
    $http_code = 400; 
    $Y['msg_stderr'] = $rts_err_msg;
  }

  header("Error-Code: $http_code");
  header("ChangeState: true");
  http_response_code($http_code);
  return $outJ;
}

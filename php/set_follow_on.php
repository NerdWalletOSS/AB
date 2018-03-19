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
require_once 'mod_cell.php'; // UTPAL: Added this line
require_once 'find_tests_to_follow.php';

function set_follow_on(
  $tid,
  $tid_to_follow
)
{
  //--- Logging 
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "set_follow_on");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = " { \"tid\" : $tid, \"$tid_to_follow\" : $tid_to_follow } ";
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;

  // TODO: Handle case where tid_to_follow is null
  // START Check inputs
  rs_assert(is_numeric($tid));
  rs_assert(is_numeric($tid_to_follow));
  $T1 = db_get_row("test", "id", $tid);
  rs_assert($T1);
  $test_name = $T1['name'];
  $external_id = $T1['external_id'];

  $state_id = $T1['state_id'];
  $state = lkp("state", $state_id, "reverse");
  rs_assert($state == "draft", "Can set follow on only in draft state");

  if ( $tid_to_follow == 0 ) { 
    mod_cell("test", "pred_id", NULL, "id = $tid ");
  }
  else {
    $T2 = db_get_row("test", "id", $tid_to_follow);
    rs_assert($T2);
    $name_to_follow = $T2['name'];
    $external_id_to_follow = $T2['external_id'];

    $channel_id = $T1['channel_id'];
    rs_assert($channel_id, "Can set follow on only for test in channel");
    $channel = lkp("channel", $channel_id, "reverse");
    rs_assert($channel);
    $F = find_tests_to_follow($channel);
    $found = false;
    foreach ( $F as $f ) { 
      if ( $f['id'] == $tid_to_follow ) { $found = true; break; }
    }
    rs_assert($found, "Not a valid test to follow");

    $X['pred_id'] = $tid_to_follow;
    $X['external_id'] = $external_id_to_follow;
    mod_row("test", $X, " where id = $tid");
  }
  //------------------------------------------
  $http_code = 200;
  $outJ["status_code"] = $Y['status_code'] = $http_code;
  $outJ["msg_stdout"] = $Y['msg_stdout'] = 
    "Test [$test_name:$tid] will follow [$name_to_follow:$tid_to_follow";
  db_set_row("log_ui_to_webapp", $request_webapp_id, $Y);
  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

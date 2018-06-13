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

function set_filters(
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
  $test_id    = get_json_element($inJ, 'id'); 
  $updater_id = get_json_element($inJ, 'updater_id');
  $has_filters = get_json_element($inJ, 'has_filters');
  $has_filters = (int)($has_filters);
  rs_assert(( ( $has_filters == 1 ) || ( $has_filters == 0 ) ));

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");
  $state_id = $T['state_id'];
  $state   = lkp("state", $state_id, "reverse");
  rs_assert( ( $state == "draft" ) || ( $state == "dormant" ) );

  // Additional check that all is well 
  $t1 = db_get_test($test_id);
  $t2 = json_decode(json_encode($t1));
  $chk_rslt = chk_test_basic($t2, true);
  rs_assert($chk_rslt);
  $X1['updated_at'] = $updated_at;
  $X1['updater_id'] = $updater_id;

  $cat_filters    = get_json_element($inJ, 'CategoricalFilters'); 
  //-- START : Database updates
  unset($X);
  $X['has_filters'] = $has_filters;
  $x = db_set_row("test", $test_id, $X);
  rs_assert($x, "db_set_row_failed");

  foreach ( $cat_filters as $k => $v ) {
    $id = $v->{'id'};
    $is_on = (int)($v->{'is_on'});
    rs_assert( ( ( $is_on == 0 ) || ( $is_on == 1 ) ) );

    unset($X);
    $X['is_on'] = $is_on;
    $x = db_set_row("cat_attr_val_test", $id, $X);
    rs_assert($x, "db_set_row failed");
  }

  //-- STOP : Database updates
  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["TestID"] = $test_id; // UTPAL: Added this line as after the completion, I need the test ID back to display the page.
  $outJ["msg_stdout"] = "Changed filter for Test $test_id ";
  db_set_row("log_ui_to_webapp", $request_webapp_id, $outJ);
  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'db_get_test.php';
require_once 'db_get_rows.php';
require_once 'test_basic.php';
require_once 'start_log.php';
require_once "add_addnl_var_info.php";
require_once "set_device_specific_variant.php";

function test_clone(
  $str_inJ
)
{
  //-- START: For logging
  $ret_val = start_log($str_inJ, "clone");
  $created_at = $ret_val['created_at'];
  $updated_at = $ret_val['updated_at'];
  $t_create   = $ret_val['t_create'];
  $t_update   = $ret_val['t_update'];
  $api_id     = $ret_val['api_id'];
  $request_webapp_id = $ret_val['request_webapp_id'];
  //-- STOP: For logging

  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); 
  rs_assert(gettype($inJ) != "string");
  rs_assert($inJ, "invalid JSON");
  // test id of existing test
  $tid           = get_json_element($inJ, 'OldTestID'); 
  $new_test_name = get_json_element($inJ, 'NewTestName'); 
  $creator       = get_json_element($inJ, 'Creator'); 
  //-----------------------------------------------
  // START Check inputs
  rs_assert(is_numeric($tid));
  $old_test = db_get_test($tid);
  $old_test_name = $old_test['name'];
  $archived_id = lkp("state", "archived");
  rs_assert($old_test, "Test ID $tid not found");
  // Check that there is no un-archived test with same name
  $x = db_get_rows("test", 
    "name = '$new_test_name' and state_id != $archived_id");
  rs_assert($x == NULL);
  // STOP Check inputs
  // unset things that need to be unset
  $old_test['id'] = NULL;
  $old_test['State'] = NULL;
  $old_test['state_id'] = NULL;
  $old_test['OverWrite'] = NULL;
  // set new stuff
  $new_test = $old_test;
  $new_test['Creator'] = $creator;
  $new_test['name'] = $new_test_name;
  $is_dev_specific = $old_test['is_dev_specific'];
  $device_cross_variant = $old_test['device_cross_variant'];

  $outJ = test_basic(json_encode($new_test));
  rs_assert($outJ, "new test not created");
  $new_test_id = $outJ['TestID'];
  rs_assert($new_test_id, "new test not created");
  $save_outJ = $outJ;

  $new_test = db_get_test($new_test_id);
  $new_test['Updater'] = $creator;
  $new_test['is_dev_specific'] = $is_dev_specific;
  $new_test['device_cross_variant'] = $device_cross_variant;

  /* TODO 
  $outJ = add_addnl_var_info(json_encode($new_test));
  print("=================\n"); var_dump($outJ);

  $outJ = set_device_specific_variant(json_encode($new_test));
  print("=================\n"); var_dump($outJ);
   */
  return $save_outJ;
}

$in['OldTestID'] = 4;
$in['Creator'] = "joe";
$in['NewTestName'] = "clone17";

$str_inJ = json_encode($in);
$x = test_clone($str_inJ);
print("ALL DONE\n");
var_dump($x);


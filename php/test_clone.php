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

function mk_vid_map($old_test, $new_test)
{
  // return $map_old_vids_to_new;
}
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
  unset($old_test['id']);
  unset($old_test['State']);
  unset($old_test['state_id']);
  unset($old_test['OverWrite']);
  // set new stuff
  $new_test = $old_test;
  $new_test['Creator'] = $creator;
  $new_test['name'] = $new_test_name;
  $is_dev_specific = $old_test['is_dev_specific'];
  $device_cross_variant = $old_test['DeviceCrossVariant'];

  $outJ = test_basic(json_encode($new_test));
  rs_assert($outJ, "new test not created");
  $new_test_id = $outJ['TestID'];
  rs_assert($new_test_id, "new test not created");
  $save_outJ = $outJ;

  $new_test = db_get_test($new_test_id);
  $new_test['Updater'] = $creator;
  $new_test['is_dev_specific'] = $is_dev_specific;
  $new_test['device_cross_variant'] = $device_cross_variant;

  // START: Now we need to create a map from old variant IDs to new ones
  $old_variants = $old_test['Variants'];
  foreach ( $old_variants as $k=> $old_variant ) { 
    $variant_map[$old_variant['name']]['old_id'] = $old_variant['id'];
  }
  // STOP: create a map from old variant IDs to new ones
  $new_variants = $new_test['Variants'];
  foreach ( $new_variants as $k=> $new_variant ) { 
    $variant_map[$new_variant['name']]['new_id'] = $new_variant['id'];
  }
  foreach ( $variant_map as $vname => $x ) { 
    $old_id[$x['new_id']] = intval($x['old_id']);
    $new_id[$x['old_id']] = intval($x['new_id']);
  }

  $old_variants = $old_test['Variants'];
  foreach ( $old_variants as $k=> $old_variant ) { 
    $vname = $old_variant['name'];
    $old_test['Variants'][$k]['id'] = $variant_map[$vname]['new_id'];
    unset($old_test['Variants'][$k]['request_webapp_id']);
    unset($old_test['Variants'][$k]['t_update']);
    unset($old_test['Variants'][$k]['t_create']);
    unset($old_test['Variants'][$k]['updated_at']);
    unset($old_test['Variants'][$k]['created_at']);
    unset($old_test['Variants'][$k]['is_del']);
    unset($old_test['Variants'][$k]['pred_id']);
    unset($old_test['Variants'][$k]['api_id']);
    unset($old_test['Variants'][$k]['percentage']);
    unset($old_test['Variants'][$k]['is_final']);
  }
  foreach ( $old_test['DeviceCrossVariant'] as $device => $x ) {
    foreach ( $x as $k => $v ) {
      $variant_id = $v['variant_id'];
      rs_assert(isset($new_id[$variant_id]), "strange. variant_id = $variant_id");
      $old_test['DeviceCrossVariant'][$device][$k]['variant_id'] = 
        $new_id[$variant_id];
    }
  }
  $old_test['id'] = $new_test_id;
  $old_test['Updater'] = $creator;
  foreach ( $variant_map as $k => $v ) { 
    $old_test['VariantID'] = $v['new_id'];
    $outJ = add_addnl_var_info(json_encode($old_test));
  }

  unset($old_test['VariantID']);
  unset($old_test['BinType']);
  unset($old_test['BinType']);
  unset($old_test['t_create']);
  unset($old_test['t_update']);
  unset($old_test['creator_id']);
  unset($old_test['state_id']);
  unset($old_test['channel_id']);
  unset($old_test['State']);
  unset($old_test['Channel']);
  unset($old_test['CategoricalFilters']);
  $outJ = set_device_specific_variant(json_encode($old_test));
  return $save_outJ;
}

$in['OldTestID'] = 4;
$in['Creator'] = "joe";
$in['NewTestName'] = "TTTD";

$str_inJ = json_encode($in);
$x = test_clone($str_inJ);
print("ALL DONE\n");
var_dump($x);



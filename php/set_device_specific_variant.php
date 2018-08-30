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
require_once 'mod_cell.php';
require_once 'inform_rts.php';

function set_device_specific_variant(
  $str_inJ
)
{
  //--- Logging 
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "set_device_specific_variant");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;

  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); rs_assert($inJ, "invalid JSON");
  $tid = get_json_element($inJ, 'id'); 
  $T = db_get_row("test", "id", $tid);
  rs_assert($T);
  $test_name = $T['name'];
  $state_id = $T['state_id'];
  $state = lkp("state", $state_id, "reverse");
  rs_assert( ( ( $state != "archived" ) && ( $state != "terminated" ) ),
    "Test $test_name NO CHANGE TO DEVICE SPECIFIC ");
  if ( ( $state != "draft" ) && ( $state != "dormant" ) ) {
    // cannot change device specific once test has started
    $is_dev_specific = $T['is_dev_specific'];
  }
  else {
    $is_dev_specific = intval(get_json_element($inJ, 'is_dev_specific'));
  }
    if ( !is_bool($is_dev_specific) ) { 
      if ( $is_dev_specific == 0 ) { 
        $is_dev_specific = false;
      }
      else if ( $is_dev_specific == 1 ) { 
        $is_dev_specific = true;
      }
      else {
        rs_assert(false, "is_dev_specific invalid = $is_dev_specific");
      }
    }
  $dxv = get_json_element($inJ, 'DeviceCrossVariant'); 
  rs_assert($dxv);
  if ( $is_dev_specific === false ) { 
    if ( ( $state == "draft" ) || ( $state == "dormant" ) ) {
      // TODO P1 Put updates in transaction
      mod_cell("test", "is_dev_specific", $is_dev_specific, " id = $tid ");
      $outJ["msg_stdout"] = "Test $test_name DEVICE SPECIFIC SET OFF";
    }
    else {
      $outJ["msg_stdout"] = "Test $test_name NO CHANGE TO IS DEVICE SPECIFIC";
    }
    $outJ["TestID"] = $tid; 
    return $outJ;
  }
  // TODO P1 Put updates in transaction
  mod_cell("test", "is_dev_specific", $is_dev_specific, " id = $tid ");
  foreach ( $dxv as $d => $v ) { 
    $device_id   = lkp("device", $d);
    rs_assert(is_array($v));
    $sum = 0;
    foreach ( $v as $k => $vv ) { 
      $result = var_export($vv, true);
      $result = str_replace("\n", "__", $result);
      $did = $vv->{'device_id'};
      rs_assert($did, $result);
      $vid = $vv->{'variant_id'};
      rs_assert($vid, $result);
      $p = $vv->{'percentage'};
      // TODO P4 Why this hackery?
      if ( (int)$p != 0 ) { 
        rs_assert($p, "percentage not provided, $vid, $did, [$result]");
      }
      rs_assert(is_numeric($p), "percentage must be a number. Got [$p]");
      rs_assert($p >=   0, "percentage must not be negative");
      rs_assert($p <= 100, "percentage cannot exceed 100");
      $sum += $p;
    }
    rs_assert(( $sum < 100 + 0.0001 ) && ( $sum > 100 - 0.0001 ) );
    foreach ( $v as $vv ) { 
      $p = $vv->{'percentage'};
      if ( is_null($p) ) { $p = $vv['percentage']; }
      $vid = $vv->{'variant_id'};
      if ( is_null($vid) ) { $vid = $vv['variant_id']; }
      $did  = $vv->{'device_id'};
      if ( is_null($did) ) { $did = $vv['device_id']; }
      mod_cell("device_x_variant", "percentage", $p, 
        " device_id = $did and variant_id = $vid ");
    }
  }
  //------------------------------------------
  $outJ["msg_stdout"] = "SET Device Specific Variants for [$test_name] ";
  $outJ["TestID"] = $tid; 
  return $outJ;
}

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "dbconn.php";
require_once "db_num_rows.php";
require_once "db_set_row.php";
require_once "db_get_row.php";
require_once "db_get_rows.php";
require_once "name_to_id.php";
require_once "get_time_usec.php";
require_once "insert_row.php";
require_once "make_seed.php";
require_once "load_configs.php";
require_once "db_logger.php";

function clone_test(
  $test_id, // ID of test 
  $creator_id
)
//</hdr>
{
  $new_test_id = -1;
  $status = false;
  $GLOBALS["err"] = "";
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $configs = $GLOBALS['CONFIGS'];
  $server                = $configs['server'];
  $port                  = $configs['port'];

  //--- Some utility stuff for use later
  $t_date = get_date();
  $t_time = get_time_usec();
  $dormant_id = name_to_id("state", "name", "dormant");
  $terminated_id = name_to_id("state", "name", "terminated");
  $archived_id = name_to_id("state", "name", "archived");
  $xy_test_type_id = name_to_id("test_type", "name", "XYTest");
  $x = db_get_row("admin", "pk", $creator_id);
  if ( !$x ) { 
    $GLOBALS["err"] .= "Cannot connect to database\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  //---------------------------------------
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "Cannot connect to database\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  //---------------------------------------
  if ( !is_numeric($test_id) ) { 
    $GLOBALS["err"] .= "Invalid TestID [$test_id]\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $T1 = db_get_row("abtest", "pk", $test_id);
  if ( !$T1 ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  // Some basic testing
  if ( $T1['state_id'] != $terminated_id )  {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  if ( $T1['test_type_id'] != $xy_test_type_id)  {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  //--------------------
  $test_name                = $T1['name'];
  $T1['pk']                 = null;
  $T1['pred_id']            = $test_id;
  $T1['state_id']           = $dormant_id;
  $T1['ramp_num']           = 1;
  $T1['date_created']       = $t_date;
  $T1['seed']               = make_seed();
  $T1['t_create']           = $t_time;
  $T1['most_recent_update'] = $t_date;
  $T1['t_update']           = $t_time;
  $T1['creator_id']         = $creator_id;
  $new_test_id = insert_row("abtest", $T1); 
  if ( !$new_test_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  //--========================================
  $V1 = db_get_rows("variant", "abtest_id = $test_id");
  if ( !$V1 ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $vidx = 0;
  foreach ( $V1 as $v1 ) {
    $old_variant_id = $v1['pk'];
    $v1['pk']        = null;
    $v1['abtest_id'] = $new_test_id;
    $v1['count']     = null;
    $v1['is_final']  = false;
    $v1['dwh_count'] = null;
    $v1['t_update']  = $t_time;
    $new_variant_id = insert_row("variant", $v1); 
    if ( !$new_variant_id ) { 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
    $new_variant_ids[$old_variant_id] = $new_variant_id;
    $old_variant_ids[$vidx]           = $old_variant_id;
    $vidx++;
  }
  //--========================================
  $S1 = db_get_rows("state_history", 
    "abtest_id = $test_id and ramp_num = 1");
  if ( !$S1 ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  foreach ( $S1 as $s1 ) {
    $s1['pk']           = null;
    $s1['abtest_id']    = $new_test_id;
    $s1['modifier_id']  = $creator_id;
    $s1['state_id']     = $dormant_id;
    $s1['when_changed'] = $t_date;
    $s1['t_change']     = $t_time;
    $temp = insert_row("state_history", $s1); 
    if ( !$temp ) { 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
  }
  //--========================================
  $vidx = 0;
  foreach ( $V1 as $v1 ) {
    $old_variant_id       = $v1['pk'];
    $v1['pk']             = null;
    $v1['ramp_num']       = 1;
    $v1['abtest_id']      = $new_test_id;
    $v1['modifier_id']    = $creator_id;
    $v1['variant_id']     = $new_variant_ids[$old_variant_id];
    $v1['when_changed']   = $t_date;
    $v1['t_change']       = $t_time;
    // percentage as is 
    $v1['count']          = 0;
    $v1['state_id']       = $dormant_id;
    //---- not needed from variant
    $v1['name']           = null;
    $v1['is_final']       = null;
    $v1['description']    = null;
    $v1['dwh_count']      = null;
    $v1['url']            = null;
    $v1['t_update']       = null;
    $v1['custom_data']    = null;
    $temp = insert_row("ramp_history", $v1); 
    if ( !$temp ) { 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
    $vidx++;
  }
  //--========================================
  $vids_as_set  = " ( ";
  $first =  true;
  foreach ( $old_variant_ids as $k => $v ) { 
    if ( $first ) { 
      $first = false;
    }
    else { 
      $vids_as_set .= " , ";
    }
    $vids_as_set .= " $v ";
  }
  $vids_as_set .= " ) ";
  // print("vids as set =  $vids_as_set \n");
  $DxV = db_get_rows("device_x_variant", "ramp_num = 1 and variant_id in $vids_as_set ");
  if ( !$DxV ) { 
    // print("No device specific variants for this test $test_name\n");
  }
  else {
    foreach ( $DxV as $dxv ) {
      $dxv['pk']             = null;
      // device_id as is
      $old_variant_id        = $dxv['variant_id'];
      $dxv['variant_id']     = $new_variant_ids[$old_variant_id];
      // percentage as is 
      $dxv['ramp_num']       = 1;
      $temp = insert_row("device_x_variant", $dxv); 
      if ( !$temp ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $status = false; goto BYE;
      }
    }
  }
  //--========================================
  $rslt = db_set_row("abtest", "state_id", $archived_id, 
    " where pk = $test_id");
BYE:
  $test_name = id_to_name($test_id, "abtest");
  $creator = id_to_name($creator_id, "admin");
  $J = " { \"TestName\" : \"$test_name\", ";
  $J .= " \"TestID\" : \"$test_id\", ";
  $J .= " \"Creator\" : \"$creator\" } ";
  db_logger($J, $status, "CloneTest");
  if ( $status ) { return $new_test_id; } else { return null; }
}
/* $new_test_id = clone_test(1246, 1);
$new_test_id = clone_test(1146, 1);
if ( !$new_test_id ) { 
  echo "FAILURE\n"; var_dump($GLOBALS['err']); 
}
else {
  echo "new test = $new_test_id\n";
}
 */
?>

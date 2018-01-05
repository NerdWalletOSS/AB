<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_get_variant.php";
require_once "get_time_usec.php";
require_once "xurl.php";
require_once "rts_func_lib.php";
require_once "load_configs.php";
require_once "chk_terminate_is_dev_specific.php";

function chk_set_state(
  $test_name,
  $test_type, 
  &$new_state,
  $modifier,
  $final_variant,
  &$abtest_id,
  &$ramp_num,
  &$final_variant_id,
  &$new_state_id,
  &$modifier_id,
  &$is_change
)
{
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  if ( !$configs ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $dbh = dbconn();  
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //---------------------------------------------
  $test_name = trim($test_name);
  $test_type = strtolower(trim($test_type));
  $new_state = strtolower(trim($new_state));
  $final_variant = trim($final_variant);
  $modifier = trim($modifier);
  //---------------------------------------------
  $test_type_id = name_to_id("test_type", "name", $test_type);
  if ( is_null($test_type_id) ) { 
    $GLOBALS["err"] .= "Invalid test type [$test_type]";  
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //---------------------------------------------
  $archived_id = name_to_id("state", "name", "archived");
  $new_state_id  = name_to_id("state", "name", $new_state);
  if ( !$new_state_id ) { 
    $GLOBALS["err"] .= "Invalid new_state [$new_state]";  return false; 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  }
  //---------------------------------------------
  $T = db_get_row("abtest", "name", $test_name, 
    " and state_id != $archived_id  and test_type_id = $test_type_id ");
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid state transition for Test [$test_name] to $new_state\n"; 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $abtest_id     = $T['pk'];
  $ramp_num      = $T['ramp_num'];
  $is_private    = $T['is_private'];
  $curr_state_id = $T['state_id']; 
  $creator_id    = $T['creator_id']; 
  $is_dev_specific    = $T['is_dev_specific']; 
  $curr_state    = id_to_name($curr_state_id, "state");


  if ( ( $is_dev_specific == 1 ) && ( $test_type == "xytest" ) && 
    ( $new_state == "terminated" ) ) {
    $ok = chk_terminate_is_dev_specific($test_name);
    if ( !$ok ) { return false; }
  }
  if ( $new_state == $curr_state ) { 
    $GLOBALS["err"] .= "WARN: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $is_change = false;
    return true;
  }
  $is_change = true;
  $invalid_trans = false;
  if ( ( $curr_state == "dormant" ) && ( $new_state != "started" ) ) { 
    $invalid_trans = true;
  }
  if ( ( $curr_state == "terminated" ) && ( $new_state != "archived" ) ) { 
    $invalid_trans = true;
  }
  if ( ( $curr_state == "started" ) && ( $new_state != "terminated" ) ) { 
    $invalid_trans = true;
  }
  if ( $invalid_trans ) { 
    $GLOBALS["err"] .= "Invalid state transition for Test [$test_name]\n";
    $GLOBALS["err"] .= "From $curr_state to $new_state \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //--------------------------------------------------
  if ( $new_state == "terminated" ) {
    $V = db_get_row("variant", "name", $final_variant, 
      " and abtest_id = $abtest_id");
    if ( !$V ) { 
      $GLOBALS["err"] .= "Invalid final variant [$final_variant]\n";
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
    $final_variant_id = $V['pk'];
  }
  //--------------------------------------------------
  $modifier_id = name_to_id("admin", "name", $modifier);
  if ( !$modifier_id ) { 
    $GLOBALS["err"] .= "Invalid admin [$modifier]\n"; 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  if ( ( $is_private == 1 ) && ( $modifier_id != $creator_id ) ) {
    $GLOBALS["err"] .= "User [$modifier] not creator\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  return true;
}
?>

<?php
require_once "webapp_api.php";
  require_once "xurl.php";

function get_counts_from_rts(
  $in_test_name,
  $test_type,
  &$msg
)
{
  $msg = "";
  $rslt = db_get_row("test_type", "name", $test_type);
  if ( !$rslt ) { 
    $msg = "ERR: Invalid test type = [$test_type]"; return false;
  }
  //---------------------------------------------------
  // Load configs and quit early if not possible to do so 
  if ( !load_configs() ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  if ( !$configs ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $server = $configs['server'];
  $port   = $configs['port'];
  if ( ( $server == "" ) || ( $port == "" ) )  {
    $GLOBALS["err"] = "ERROR: WARNING Not talking to RTS";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  //---------------------------------------------------
  $is_archived = false; 
  $db_tests  = db_get_tests($test_type, $is_archived);
  $nR = count($db_tests);
  if ( $nR == 0 ) { 
    $msg = "Nothing to synchronize"; return true; 
  }
  $rts_tests = rts_list_tests($server, $port, $test_type);
  if ( is_null($rts_tests) ) {
    $msg = "ERROR: RTS not available"; return false;
  }
  if ( $nR != count($rts_tests) ) {
    $msg = "ERROR: Mismatch in NumTests between DB and RTS";
    $msg .= "test_type = $test_type";
    return false;
  }
  for ( $i = 0; $i < $nR; $i++ ) {
    $db_test   = $db_tests[$i];
    $test_name = $db_test['name'];
    if ( ( $in_test_name != "" ) && ( $test_name != $in_test_name ) ) {
      continue;
    }
    $ramp_num = $db_test['ramp_num'];
    $abtest_id = $db_test['pk'];
    $counts = get_counts($server, $port, $test_name, $test_type);
    if ( !$counts ) { 
      $GLOBALS["err"] = "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $rslt = set_counts($abtest_id, $ramp_num, $counts);
    if ( !$rslt ) {
      $msg = "set_counts() failed \n"; return false; 
    }
  }
  $msg = "RTS COUNT UPDATED SUCCESSFULLY"; return true;
}
?>

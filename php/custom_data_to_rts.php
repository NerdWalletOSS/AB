<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "name_to_id.php";
require_once "db_get_row.php";
require_once "yurl.php";
require_once "list_rts.php";
require_once "db_get_variants.php";
require_once "load_configs.php";

function custom_data_to_rts(
  $test_name
)
{
  $GLOBALS['err'] = "";
  $is_ok = true;
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $SP = list_rts();
  if ( !is_array($SP) ) { // NOTHING MORE TO DO 
    $GLOBALS["err"] .= "NOTICE: Not talking to RTS\n";
    return true;  
  } 
  $dbh = dbconn();  
  // START: Error checking on inputs 
  $test_type_id = name_to_id("test_type", "name", "ABTest");
  $archived_id = name_to_id("state", "name", "archived");
  $T = db_get_row("abtest", "name", $test_name, 
    " and state_id != $archived_id  and test_type_id = $test_type_id ");
  if (!$T ) { 
    $GLOBALS["err"] .= "No Test [$test_name] \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $abtest_id = $T['pk'];
  $state_id  = $T['state_id'];
  $state = id_to_name($state_id, "state");
  if ( $state == "dormant" ) { 
    // Nothing to do 
    return true;
  }
  $V = db_get_variants($abtest_id);
  //----------------------------------------------------
  foreach ( $SP  as $sp ) { 
    $rslt = ""; $http_code = 0;
    $server = $sp['server']; $port   = $sp['port'];
    foreach ( $V as $v ) {
      $url = "SetCustomData?TestName=$test_name";
      $url .= "&Variant=" . $v['name'];
      if ( $v['custom_data'] != "" ) { 
        $url .= "&CustomData=" . urlencode($v['custom_data']);
      }
      yurl($server, $port, $url, $http_code, $rslt);
      if ( $http_code != 200 ) { 
        $GLOBALS["err"] .= "Error [$rslt] from RTS on $url\n"; 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $is_ok = false;
      }
    }
  }
  return $is_ok;
}
/*
$x = custom_data_to_rts("T1");
var_dump($x);
var_dump($GLOBALS['err']);
 */
?>

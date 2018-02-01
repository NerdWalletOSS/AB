<?php
// TODO Serious updates needed to this code
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "xurl.php";
require_once "rts_func_lib.php";
require_once "load_configs.php";
require_once "db_get_tests.php";
require_once "db_get_rows.php";
require_once "chk_constants.php";
require_once "list_rts.php";
require_once "name_to_id.php";
require_once "db_get_variants.php";

function chk_rts()
{
  $GLOBALS["err"] = "";
  $SP = list_rts();
  if ( !$SP ) {
    $GLOBALS["err"] .= "No RTS specified \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  foreach ( $SP as $sp ) { 
    $server = $sp['server']; $port   = $sp['port'];
    $rslt = l_chk_rts($server, $port);
    if ( !$rslt ) { 
      $GLOBALS["err"] .= "Error with RTS $server, $port \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
  }
  return true;
}

function l_chk_rts(
  $server,
  $port
)
{
  $http_code = 0; $rslt = "";
  post_url($server, $port, "Ignore", $http_code, $rslt);
  if( !$rslt ) {
    $GLOBALS["err"] .= "Timeout when trying to connect to RTS \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false; 
  }
  //-----------------------
  $is_archived = false;
  $err = "";
  $started_id    = name_to_id("state", "name", "started");
  $terminated_id = name_to_id("state", "name", "terminated");
  $T = db_get_rows("abtest", 
    " ( ( state_id = $started_id or state_id = $terminated_id ) ) ");
  $http_code = 0; $rslt = "";
  post_url($server, $port, "NumTests", $http_code, $rslt);
  if( $http_code != 200 ) { 
    $GLOBALS["err"] .= "$rslt\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false; 
  }
  $X = json_decode($rslt);
  if ( !$X ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false; 
  }
  $NumTests = $X->{'NumTests'};
  if ( !$T ) {
    if  ( $NumTests == 0 ) { 
      return true;
    }
    else {
      $GLOBALS["err"] .= "RTS has tests that WebApp does not\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
  }
  if ( $NumTests != count($T) ) { 
    $GLOBALS["err"] .= "Mismatch in number of tests \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false; 
  }
  // --- Nowe we know that WebApp and RTS both have tests 
  foreach ( $T as $t ) {
    $test_name    = $t['name'];
    $test_type = id_to_name($t['test_type_id'], "test_type");
    $state     = id_to_name($t['state_id'],     "state");
    $http_code = 0; $rslt = "";
    post_url($server, $port, 
      "TestInfo?TestName=$test_name&TestType=$test_type", $http_code, $rslt);
    if( $http_code != 200 ) { 
      $GLOBALS["err"] .= "RTS does not know of Test [$test_name] \n"; 
      $GLOBALS["err"] .= "$rslt\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    $X = json_decode($rslt);
    //--------------------------------------------------
    if ( strtolower($state) != strtolower($X->{'State'}) ) {
      $GLOBALS["err"] .= "Mismatch in state for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( $t['is_sequential'] != $X->{'IsSequential'} ) {
      $GLOBALS["err"] .= "Mismatch in is_sequential for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( $t['ramp_num'] != $X->{'RampNum'} ) {
      $GLOBALS["err"] .= "Mismatch in ramp_num for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( $t['num_variants'] != $X->{'NumVariants'} ) {
      $GLOBALS["err"] .= "Mismatch in num_variants for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( $t['uses_eee'] != $X->{'UsesEEE'} ) {
      $GLOBALS["err"] .= "Mismatch in uses_eee id for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( $t['pk'] != $X->{'ExternalTestID'} ) {
      $GLOBALS["err"] .= "Mismatch in test id for Test $test_name\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false; 
    }
    //--------------------------------------------------
    if ( isset($t['external_id']) ) {
      if ( $X->{'ExternalID'}  != $t['external_id'] ) { 
        $GLOBALS["err"] .= "Mismatch in ExternalID. Test $test_name\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
        return false; 
      }
    }
    else {
      if ( $X->{'ExternalID'}  != 0 ) { 
        $GLOBALS["err"] .= "Mismatch in ExternalID. Test $test_name\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
        return false; 
      }
    }
    $V1 = db_get_variants($t['pk']);
    $V2 = $X->{'Variants'};
    foreach ( $V1 as $v1 ) { 
      $found = false;
      $name_v1 = $v1['name'];
      $perc_v1 = $v1['percentage'];
      $dscr_v1 = $v1['description'];
      foreach ( $V2 as $v2 ) { 
        if ( $name_v1 == $v2->{'Name'} ) {
          $found = true;
          if ( $test_type == "XYTest" ) { 
            if ( $dscr_v1 != $v2->{'URL'} ) {
              $GLOBALS["err"] .= "Mismatch in variants for Test $test_name\n";
              $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
              return false; 
            }
          }
          break;
        }
      }
      if ( !$found ) {
        $GLOBALS["err"] .= "Mismatch in variants for Test $test_name\n";
        $GLOBALS["err"] .= "Variant not found = $name_v1 \n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
        return false; 
      }
    }
  }
  //--------------------------------------------------
  if ( !chk_constants() ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "ERROR: Mismatch in constants\n";
    return false;
  }
  $GLOBALS["err"] .= "RTS and WebApp in Sync";
  return true;
}
/*
$x = chk_rts();
var_dump($x);
var_dump($GLOBALS['err']);
 */
?>

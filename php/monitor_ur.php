<?php
// TO BE WRITTEN BY UTPAL
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../daft/");

require_once "xurl.php";
require_once "bye.php";
require_once "add_test.php";
require_once "load_configs.php";
require_once "get_random_string.php";
require_once "set_ramp.php";
require_once "del_test.php";

function monitor_ur( ) 
{
  if ( !load_configs() ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  $c = $GLOBALS["CONFIGS"];
  $S = $c["server"];
  $P = $c["port"];

  /* Add a test of type XYTest with 4 variants.
   * Let proportions be 60, 20, 15, 5  */
  $valid_chars = "abcdefghijklmnopqrstwxyzABCDEFGHIJKLMNOPQRSTWXYZ";
  $admin     = "joe";
  $test_type = "XYTest"; 
  $test_name = get_random_string($valid_chars, 31);  // hopefully unique

  //-- Add a test from the webapp side
  $var_names = array('Yellow', 'Red', 'Blue', 'Green');
  $var_percs = array(60, 20, 15, 5);
  $var_texts = array(
  'http://173.255.252.157:8080/AB/UR/tcc/landing_page_1.php',
  'http://173.255.252.157:8080/AB/UR/tcc/landing_page_2.php',
  'http://173.255.252.157:8080/AB/UR/tcc/landing_page_3.php',
  'http://173.255.252.157:8080/AB/UR/tcc/landing_page_4.php'
  );
  $rslt = add_test($test_name, $test_type, "Test_Description", 
    $admin, $var_names, $var_percs, $var_texts);
  if ( !$rslt ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
        return false; 
    }
/*
  // Do GetVariant. should get status 400
  $url = "GetVariant?TestName=$test_name&TestType=$test_type";
  $rslt = xurl($S, $P, $url, 400 );
  if ( !$rslt ) { 
        $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
        return false; 
    }

  // SetState to Started.
  $rslt = set_state($test_name, $test_type, "Started", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
*/
  // Do 10000 calls to GetVariant
  for ( $iter = 0; $iter < 10000; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $X = json_decode($rslt);
    if ( !$X) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
  }

  // Do GetCounts
  // RTS: GetCounts
  $rslt = xurl($S, $P, "GetCounts?TestName=$test_name&TestType=$test_type");
  $r1 = json_decode($rslt); 
  if ( !$r1 ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  /*
   * Counts should be in 
   * range [600 +/- 30] for Variant 0
   * range [200 +/- 20] for Variant 1
   * range [150 +/- 15] for Variant 2
   * range [ 50 +/-  5] for Variant 3
   */

  $count_control = $r1->{'Yellow'};
  if ( ($count_control < 5700 ) || ($count_control > 6300))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
  } else {  // Do Nothing
  }
  $count_red = $r1->{'Red'}; 
  if ( ($count_red < 1800 ) || ($count_red > 2200))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_blue = $r1->{'Blue'}; 
  if ( ($count_blue < 1350 ) || ($count_blue > 1650))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_green = $r1->{'Green'}; 
  if ( ($count_green < 400 ) || ($count_green > 600))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
/*
  // SetState to Suspended.
  $rslt = set_state($test_name, $test_type, "Suspended", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  /* Do GetVarant
   * Should get Control 
  $uuid = 1;
  for ( $iter = 0; $iter < 1000; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type&UUID=$uuid";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
    $X = json_decode($rslt);
    if ( !X ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
    if ( $X->{'Variant'} != "Control"  ) {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
    $uuid++;
  }

  // SetState to Started.
  $rslt = set_state($test_name, $test_type, "Started", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
*/
  // Do ramp to (5, 15, 20, 60)
  $is_change = false;
  $rslt = set_ramp($test_name, $test_type, "Yellow:25 Red:25 Blue:20 Green:30", $admin, $is_change);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  // Do ramp back to (60, 20, 15, 5)
  $is_change = false;
  $rslt = set_ramp($test_name, $test_type, "Yellow:60 Red:20 Blue:15 Green:5", $admin, $is_change);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false;
  }

  // Do another 10000 calls
  for ( $iter = 0; $iter < 10000; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $X = json_decode($rslt);
    if ( !$X) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
  }

  /* Do GetCounts */
  $rslt = xurl($S, $P, "GetCounts?TestName=$test_name&TestType=$test_type");
  $r2 = json_decode($rslt); 
  if ( !$r2 ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
   /* Counts should be as above */
  $count_control = $r2->{'Yellow'};
  if ( ($count_control < 5700 ) || ($count_control > 6300))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
  } else {  // Do Nothing
  }
  $count_red = $r2->{'Red'}; 
  if ( ($count_red < 1800 ) || ($count_red > 2200))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_blue = $r2->{'Blue'}; 
  if ( ($count_blue < 1350 ) || ($count_blue > 1650))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_green = $r2->{'Green'}; 
  if ( ($count_green < 400 ) || ($count_green > 600))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
 
  // SetState to Terminated with FinalState=Variant1
  $rslt = set_state($test_name, $test_type, "Terminated", $admin, "Yellow");
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  /* Do another 10000 calls
   * Should get back Variant1 for all calls */
  for ( $iter = 0; $iter < 10000; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $X = json_decode($rslt);
    if ( !$X) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
  }

  /* Do GetCounts
   * Counts should be as above i.e, once test is terminated, further calls to 
   * GetVariant do not change counts */
  $rslt = xurl($S, $P, "GetCounts?TestName=$test_name&TestType=$test_type");
  $r3 = json_decode($rslt); if ( !$r3 ) {
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  foreach ( $r2 as $name => $count ) { 
    if ( $r3->{$name} != $count ) {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
  }

  // Run Diagnostics. Should get status 200
  $rslt = xurl($S, $P, "Diagnostics");
  $X = json_decode($rslt);
  if ( !$X) { 
  $GLOBALS["err"] = $r3." FILE: " . __FILE__ . ": LINE: " . __LINE__;
  return true; 
  }
  if ($X->{'Diagnostics'} != "OK")  {
    $GLOBALS["err"] = $r3." FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return true; 
  }

  // Archive test
  $rslt = set_state($test_name, $test_type, "Archived", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
/*
  // Do GetVariant, should not get status 200
  $url = "GetVariant?TestName=$test_name&TestType=$test_type";
  $rslt = xurl($S, $P, $url, 200 );
  if ( $rslt ) { 
  $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
  return false; 
  }

  // Get nw_campaign_id through RTS
  $rslt = xurl($S, $P, "TestInfo?TestName=$test_name&TestType=$test_type");
  $X = json_decode($rslt);  if ( !$X ) {
  $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  $nw_campagn_id = $X->{'CreateTime'};

  // Test UR Router through RTS
  $rslt = xurl($S, $P, "Router?nw_campaign_id=$nw_campagn_id");
  if ( !$rslt) { 
  $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
*/
  /* hard delete of all rows for this test in all tables
   * abtest
   * variant
   * ramp_history
   * state_history */
  $x = del_test($test_name, $test_type);
  if ( !$x ) { 
  $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
  return false; 
  }

  /* If success, should echo { "Monitor" : "OK" }. Else, should echo 
   * { "Monitor" : "ERROR", "Message" : "<LINE NUMBER OF ERROR>" }
   * */

  return true;

}
?>

<?php
// TO BE WRITTEN BY UTPAL
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../daft/");

require_once "xurl.php";
require_once "yurl.php";
require_once "bye.php";
require_once "add_test.php";
require_once "load_configs.php";
require_once "get_random_string.php";
require_once "set_ramp.php";
require_once "del_test.php";

function monitor( ) 
{
  if ( !load_configs() ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  $c = $GLOBALS["CONFIGS"];
  $S = $c["server"];
  $P = $c["port"];

  /* Add a test of type ABTest with 4 variants (Control+3)
   * Let proportions be 60, 20, 15, 5  */
  $valid_chars = "abcdefghijklmnopqrstwxyzABCDEFGHIJKLMNOPQRSTWXYZ";
  $admin     = "joe";
  $test_type = "ABTest"; 
  $test_name = get_random_string($valid_chars, 31);  // hopefully unique

  //-- Add a test from the webapp side
  $var_names = array('Control', 'Red', 'Blue', 'Green');
  $var_percs = array(60, 20, 15, 5);
  $var_texts = array('Control', 'Red_Description', 'Blue_Description', 'Green_Description');
  $rslt = add_test($test_name, $test_type, "Test_Description", 
    $admin, $var_names, $var_percs, $var_texts);
  // echo "Monitor added $test_name\n";
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

  $num_trials = 10000;
  // Do $num_trials calls to GetVariant with UUID=1, 2. ... $num_trials
  $uuid = 1;
  for ( $iter = 0; $iter < $num_trials; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type&UUID=$uuid";
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
    $uuid++;
  }
  $rslt = xurl($S, $P, "Diagnostics");

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
   * range [600 +/- 30] for Control
   * range [200 +/- 20] for Variant 1
   * range [150 +/- 15] for Variant 2
   * range [ 50 +/-  5] for Variant 3
   */

  $count_control = $r1->{'Control'};
  if ( ($count_control < 0.57*$num_trials ) || ($count_control > 0.63*$num_trials))  {
    var_dump($r1);
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
  } else {  // Do Nothing
  }
  $count_red = $r1->{'Red'}; 
  if ( ($count_red < 0.18*$num_trials ) || ($count_red > 0.22*$num_trials))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_blue = $r1->{'Blue'}; 
  if ( ($count_blue < 0.135*$num_trials ) || ($count_blue > 0.165*$num_trials))  {
    var_dump($r1);
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_green = $r1->{'Green'}; 
  if ( ($count_green < 0.04*$num_trials ) || ($count_green > 0.06*$num_trials))  {
    var_dump($r1);
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }

  // SetState to Suspended.
  $rslt = set_state($test_name, $test_type, "Suspended", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  /* Do GetVarant
   * Should get Control */
  $uuid = 1;
  for ( $iter = 0; $iter < $num_trials; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type&UUID=$uuid";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
    $X = json_decode($rslt);
    if (!$X ) { 
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

  // Do ramp to (5, 15, 20, 60)
  $is_change = false;
  $rslt = set_ramp($test_name, $test_type, "Control:25 Red:25 Blue:20 Green:30", $admin, $is_change);
  if ( !$rslt ) { 
    var_dump($GLOBALS["err"]);
    debug_print_backtrace(); 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  // Do ramp back to (60, 20, 15, 5)
  $is_change = false;
  $rslt = set_ramp($test_name, $test_type, "Control:60 Red:20 Blue:15 Green:5", $admin, $is_change);
  if ( !$rslt ) { 
    var_dump($GLOBALS["err"]);
    debug_print_backtrace(); 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false;
  }

  // Do another $num_trials calls with UUID = $num_trials+1, ... 2*num_trials
  $uuid = $num_trials+1;
  for ( $iter = 0; $iter < $num_trials; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type&UUID=$uuid";
    $err_msg = ""; $http_code = 0;
    yurl($S, $P, $url, $http_code, $err_msg);
    if ( $http_code != 200 ) { 
      $GLOBALS["err"]  = "RTS code = $http_code, Msg = [$err_msg]";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    }
    $X = json_decode($rslt);
    if ( !$X) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $uuid++;
  }

  /* Do GetCounts */
  $rslt = xurl($S, $P, "GetCounts?TestName=$test_name&TestType=$test_type");
  $r2 = json_decode($rslt); 
  if ( !$r2 ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
   /* Counts should be as above */
  $count_control = $r2->{'Control'};
  if ( ($count_control < 0.57*$num_trials ) || ($count_control > 0.63*$num_trials))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
  } else {  // Do Nothing
  }
  $count_red = $r2->{'Red'}; 
  if ( ($count_red < 0.18*$num_trials ) || ($count_red > 0.22*$num_trials))  {
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_blue = $r2->{'Blue'}; 
  if ( ($count_blue < 0.135*$num_trials ) || ($count_blue > 0.165*$num_trials))  {
    var_dump($r2);
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
  $count_green = $r2->{'Green'}; 
  if ( ($count_green < 0.04*$num_trials ) || ($count_green > 0.06*$num_trials))  {
    var_dump($r2);
      $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
      return false; 
    } else {  // Do Nothing
    }
 
  // SetState to Terminated with FinalState=Variant1
  $is_change = false;
  $rslt = set_state($test_name, $test_type, "Terminated", $admin, $is_change, "Red");
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  /* Do another $num_trials calls with UUID = $num_trials+1, ... 2*num_trials
   * Should get back Variant1 for all calls */
  $uuid = $num_trials+1;
  for ( $iter = 0; $iter < $num_trials; $iter++ ) {
    $url = "GetVariant?TestName=$test_name&TestType=$test_type&UUID=$uuid";
    $rslt = xurl($S, $P, $url, 200 );
    if ( !$rslt ) { 
    $GLOBALS["err"]  = "URL=[$url]";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $X = json_decode($rslt);
    if ( !$X) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
    }
    $uuid++;
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
  $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
  return false; 
  }
  if ($X->{'Diagnostics'} != "OK")  {
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  // Archive test
  $rslt = set_state($test_name, $test_type, "Archived", $admin);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }

  /* hard delete of all rows for this test in all tables
   * abtest
   * variant
   * ramp_history
   * state_history */
  // TODO: Uncomment this later
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

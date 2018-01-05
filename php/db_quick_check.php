<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "name_to_id.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "db_get_tests.php";
require_once "aux_check_name.php";
require_once "db_get_variants.php";
require_once "db_get_rows.php";
require_once "chk_url.php";
require_once "load_configs.php";

//<hdr>
// Some quick checks on whether database is in consistent state
function db_quick_check(
)
{
  $GLOBALS["err"] = "";
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $min_num_variants = $configs['min_num_variants'];
  $max_num_variants = $configs['max_num_variants'];
  if ( $configs['check_url_reachable'] == "true" ) {
    $check_url_reachable = true;
  }
  else {
    $check_url_reachable = false;
  }
  $dbh = dbconn(); 
  if ( !$dbh ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //---------------------------------------------------
  $archived_id     = name_to_id("state", "name", "archived");
  $xy_test_type_id = name_to_id("test_type", "name", "XYTest");
  $test_types = array("ABTest", "XYTest");
  foreach ( $test_types as $test_type ) {
    $T = db_get_tests($test_type); // don't get archived tests 
    if ( !$T ) { continue; } // no tests of this type 
    unset($tnames); $tidx = 0;
    foreach ( $T as $t ) {
      $tnames[$tidx] = $t['name'];
      $abtest_id       = $t['pk'];
      $test_name       = $t['name'];
      $pred_id         = $t['pred_id'];
      $is_dev_specific = $t['is_dev_specific'];
      $external_id     = $t['external_id'];
      $state         = id_to_name($t['state_id'], "state");
      //----------------------------
      if ( ( !aux_check_name($test_name) ) || 
        ( strlen($test_name) > $configs['max_len_test_name'] ) ) {
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
      }
      // Only one variant marked as final for terminated test
      // Zero variant marked as final for non terminated test
      $n = db_num_rows("variant", "abtest_id = $abtest_id and is_final = 1 ");
      if ( $state == "terminated" ) {
        if ( $n != 1 ) { 
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
      }
      else {
        if ( $n != 0 ) { 
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
      }
      //----------------------------------------
      unset($vnames); $vidx = 0;
      $V = db_get_variants($t['pk']);
      if ( !$V ) { 
        echo "No variants for test $test_name \n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      //---- CHeck number of variants
      $nV = count($V);
      if ( ( $nV < $min_num_variants ) || ( $nV > $max_num_variants ) ) {
        echo "Invalid number of variants for test $test_name \n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      $sum = 0;
      foreach ( $V as $v ) { 
        $vnames[$vidx] = $v['name'];
        if ( ( !aux_check_name($v['name']) ) || 
          ( strlen($v['name']) > $configs['max_len_variant_name'] ) ) {
            $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            return false;
          }
        if ( !aux_check_name($v['name']) ) {
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
        if ( ( $v['percentage'] < 0 ) || ( $v['percentage'] > 100 ) ) {
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
        $sum += $v['percentage'];
        if ( $test_type == "XYTest" ) { 
          if  ( ( isset($v['custom_data']) ) && 
            ( $v['custom_data'])  != "" ) {
              $GLOBALS["err"] .= "No custom data for XYTest";
              $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
              return false;
          }
          $url = $v['description'];
          if ( !chk_url_text($url) ) { 
            $GLOBALS["err"] .= "Bad URL  $url for test $test_name\n";
            $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            return false;
          }
          if ( $check_url_reachable ) {
            $x = strpos($url, "localnerd");
            if ( is_numeric($x) ) { 
              echo "Bad url x = $x, $url \n";
            }
            else {
              print_r($x);
              if ( !chk_url($url) ) { 
                $GLOBALS["err"] .= "Unable to access $url for test $test_name\n";
                $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
                return false;
              }
            }
          }

        }
        $vidx++;
      }
      // Check that percentages add to 100 for variants
      if ( $sum != 100 ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      // Check that variant names unique 
      for ( $i = 0; $i < $vidx; $i++ ) { 
        for ( $j = $i+1; $j < $vidx; $j++ ) { 
          if ( $vnames[$i] == $vnames[$j] ) { 
            $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            return false; 
          }
        }
      }
      // count number of rows in ramp history
      $ramp_num = $t['ramp_num'];
      $num_variants = $t['num_variants'];
      $expected_num_rows = ($ramp_num * $num_variants);
      $actual_num_rows = 
        db_num_rows("ramp_history", " abtest_id = $abtest_id") ;
      if ( $expected_num_rows != $actual_num_rows ) { 
        $GLOBALS["err"] .= "Error for test $abtest_id \n";
        $GLOBALS["err"] .= "expected_num_rows = $expected_num_rows\n";
        $GLOBALS["err"] .= "actual_num_rows   = $actual_num_rows\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        // return false; 
      }
      // external ID must be set for XYTest
      // external ID must NOT be set for ABTest
      if ( !is_numeric($t['external_id']) ) {
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
      switch ( $test_type ) { 
      case "ABTest" : 
        if ( $t['external_id'] != 0 ) {
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false; 
        }
        break;
      case "XYTest" : 
        if ( $t['external_id'] <= 0 ) { 
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          // return false; 
        }
        break;
      default : 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
      $tidx++;
    }
    //----  Test names must be unique for this test_type
    for ( $i = 0; $i < $tidx; $i++ ) { 
      for ( $j = $i+1; $j < $tidx; $j++ ) { 
        if ( $tnames[$i] == $tnames[$j] ) { 
          echo "Duplicate test name " . $tnames[$i] . "\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false; 
        }
      }
    }
    switch ( $state ) { 
    case "dormant"    : $expected_num_rows = 1; break;
    case "started"    : $expected_num_rows = 2; break;
    case "terminated" : $expected_num_rows = 3; break;
    case "archived"   : $expected_num_rows = 4; break;
    default :   go_BYE(""); break;
    }
    $actual_num_rows = 
      db_num_rows("state_history", " abtest_id = $abtest_id");
    if ( $expected_num_rows != $actual_num_rows ) {
      $GLOBALS["err"] .= "Error for test $abtest_id \n";
      $GLOBALS["err"] .= "expected_num_rows = $expected_num_rows\n";
      $GLOBALS["err"] .= "actual_num_rows   = $actual_num_rows\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      // return false; 
    }
    switch ( $test_type ) {
    case "XYTest" : 
      $TXY = db_get_rows("abtest", " test_type_id = $xy_test_type_id and state_id != $archived_id ");
      if ( $TXY ) {
        $tidx = 0;
        foreach ( $TXY as $txy ) { 
          if ( $txy['external_id'] == 0 ) {
            $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            // return false; 
          }
          else {
            $X[$tidx++] = $txy['external_id'];
          }
        }
        for ( $i = 0; $i < $tidx; $i++ ) {
          for ( $j = $i+1; $j < $tidx; $j++ ) { 
            if ( ( $X[$i] != 0 ) && ( $X[$i] == $X[$j] ) ) {
              $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
              return false; 
            }
          }
        }
      }
      if ( isset($pred_id) ) {
        if ( $external_id == $t_create ) { 
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
      }
      break;
    case "ABTest" : 
      if ( $external_id != 0 ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      if ( isset($pred_id) ) {
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      break;
    default : 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
      break;
    }
  }
  return true;
}
/*
$x = db_quick_check();
var_dump($x);
var_dump($GLOBALS['err']);
if ( $x ) { 
  echo "ALL WELL \n";
}
 */
?>

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "db_get_tests.php";
require_once "chk_test.php";

function chk_tests(
)
{
  $states = array( "started", "terminated");
  $test_types = array( "XYTest", "ABTest");
  foreach ( $test_types as $test_type ) { 
    $T = db_get_tests($test_type, $states);
    if (empty($T) ) { continue; }
    foreach ( $T as $t ) { 
      $test_name = $t['name'];
      $json_str  = " { \"TestName\" : \"$test_name\", "; 
      $json_str .= " \"TestType\" : \"$test_type\"} "; 
      $rslt = chk_test($json_str);
      rs_assert($rslt);
    }
  }
  return true;
}
?>

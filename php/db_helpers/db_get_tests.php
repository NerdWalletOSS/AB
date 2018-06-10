<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "lkp.php";
require_once "db_get_rows.php";

function db_get_tests(
  $test_type,
  $states
)
{ 
  $test_type_id = lkp("test_type", $test_type);
  rs_assert($test_type_id);
  $where_clause = " where test_type_id = $test_type_id and ";
  if ( is_string($states) )  {
    $state_id = lkp("state", $states);
    rs_assert($state_id);
    $where_clause .= " state_id = $state_id ";
  }
  else {
    $where_clause .= " state_id in ( ";
    rs_assert(is_array($states));
    $first = true;
    foreach ( $states as $state ) { 
      rs_assert(is_string($state));
      $state_id = lkp("state", $state);
      rs_assert($state_id);
      if ( $first ) { 
        $where_clause .= " $state_id ";
        $first = false;
      }
      else {
        $where_clause .= " , $state_id ";
      }
    }
    $where_clause .= " ) ";
  }
  //var_dump($where_clause);
  //exit;
  $T = db_get_rows("test", $where_clause);
  return $T;
}
// db_get_tests("ABTest", "started");
// db_get_tests("XYTest", array("started", "terminated"));
?>

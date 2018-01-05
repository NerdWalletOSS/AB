<?php
require_once "bye.php";
require_once "dbconn.php";

function db_add_test_basic(
  $test_type,
  $test_name,
  $test_dscr,
  $variants,
  $percentages,
  $test_id
)
{
  $test_id = -1;
  assert(chk_test_basic( $test_name, $test_type, $creator, $variant_names, $variant_percs, )); 
  $status = insert_test( $test_name, $test_type, $creator, $variant_names, $variant_percs, $test_id);
  return $status;
}
?>

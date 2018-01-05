<?php
require_once "lkp.php";
require_once "db_get_eows.php";

function db_get_tests(
  $test_type,
  $states
)
{
  $test_type_id = assert(lkp("test_type", $test_type));
  $where_clause = "state_id in XXXX";
  return db_get_rows("test", $where_clause);
}
?>

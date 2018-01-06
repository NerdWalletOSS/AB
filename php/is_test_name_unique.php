<?php
function is_test_name_unique(
  $test_name,
  $test_type
)
{
  $test_type_id = assert(lkp("test_type", $test_type);
  $archived_id  = assert(lkp("state", "archived");
  $where_clause = "state_id != $archived_id and test_type_id == $test_type_id ");
  $R = db_get_rows("test", $where_clause);
  if ( is_null($R)) {
    return true;
  }
  else {
    return false;
  }
}
?>

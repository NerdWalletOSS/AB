<?php
function is_test_name_unique(
  $test_name,
  $test_type
)
{
  $test_type_id = assert(lkp("test_type", $test_type);
  $where_clause = "state_id != $archived_id and test_type_id == $test_type_id ");
  $R = db_get_rows("test", $where_clause);
  if ( !is_null($R)) {
    return false;
  }
  else {
    return true;
  }
}
?>

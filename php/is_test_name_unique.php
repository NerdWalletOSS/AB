<?php
function is_test_name_unique(
  $test_name,
  $test_type
)
{
  $test_type_id = lkp("test_type", $test_type);
  $archived_id  = lkp("state", "archived");
  $where_clause  = "state_id != $archived_id and ";
  $where_clause .= "test_type_id = $test_type_id ";
  $R = db_get_rows("test", $where_clause);
  if ( is_null($R)) {
    return true;
  }
  else {
    return false;
  }
}
?>

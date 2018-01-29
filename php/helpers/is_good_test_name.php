<?php
require_once 'rs_assert.php';
require_once 'is_test_name_unique.php';
function is_good_test_name(
  $test_name, 
  $test_type,
  $test_id = -1
)
{
  rs_assert(aux_chk_name($test_name), "test name is invalid");
  rs_assert(strlen($test_name) <= lkp("configs", "max_len_test_name"));
  rs_assert(is_test_name_unique($test_name, $test_type, $test_id), 
  "test name [$test_name] not unique");
  return true;
}
?>

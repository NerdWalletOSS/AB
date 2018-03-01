<?php
require_once 'rs_assert.php';
require_once 'is_test_name_unique.php';
function is_good_test_name(
  $test_name, 
  $test_type,
  $test_id = -1
)
{
  rs_assert(aux_chk_name($test_name), 
  "test name is invalid. Only alphanumeric and underscore allowed");
  rs_assert(strlen($test_name) <= lkp("configs", "max_len_test_name"),
    "Test name too long. Max length is " ..  lkp("configs", "max_len_test_name"));
  return true;
}
?>

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'aux_chk_name.php';

function chk_test_basic(
  $test_name, // name of test, mandatory
  $test_type, // ABTest, XYTest
  $creator,   // name of user who is creating test
  $variant_names, // array: names of variants
  $variant_percs, // array: percentages of variants
)
{
  /*
  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= $GLOBALS['configs']['max_len_test_dscr'];
  }
   */
  assert(aux_chk_name($test_name), "test name is invalid");
  assert(lkp("test_type", $test_type));
  assert(lkp("admin", $creator));
  assert(lkp("state", "dormant"));

  assert(is_array($variant_names);
  assert(is_array($variant_percs);
  assert(count($variant_names) == count($variant_percs));
  assert(count($variant_names) >= $GLOBALS['configs']["min_num_variants"];
  assert(count($variant_names) <= $GLOBALS['configs']["max_num_variants"];
  foreach ( $variant_names as $v ) {
    assert(chk_variant_name($v), "variant name is invalid");
  }
  foreach ( $variant_percs as $p ) {
    assert(is_float($p), "percentage must be a number");
    assert($p >=   0, "percentage must not be negative");
    assert($p <= 100, "percentage cannot exceed 100");
  }
  return true;
}
?>

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
require_once 'rs_assert.php';
require_once 'is_good_percs.php';
//----------------------------------
$X = array( 10, 20, 30, 40);
$bin_type == "c_to_v_ok_v_to_c_ok_v_to_v_not_ok";
$rslt = is_good_percs($X, $bin_type);
rs_assert($rslt);
print("SUCCEEDED\n");
//----------------------------------
?>

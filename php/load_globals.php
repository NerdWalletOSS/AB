<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'db_get_rows.php';
require_once 'rs_assert.php';
function make_lkp($tbl)
{

  $R = db_get_rows($tbl);
  rs_assert(!is_null($R));
  foreach ( $R as $r ) {
    $id   = $r['id'];
    $name = $r['name'];
    $X[$name] = $id;
    $reverse_X[$id] = $name;
  }
  $GLOBALS[$tbl] = $X;
  $GLOBALS["reverse_" . $tbl] = $reverse_X;
}
//------------------------------------------
function load_globals()
{
  make_lkp("test_type");
  make_lkp("state");
  make_lkp("bin_type");
  make_lkp("admin");
  make_lkp("api");
  make_lkp("channel");
  make_lkp("device");
  make_lkp("attr");
  make_lkp("attr_type");
}
// load_globals();
?>

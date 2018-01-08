<?php
require_once 'bye.php';
require_once 'db_get_rows.php';
function make_lkp($tbl)
{

  $R = db_get_rows($tbl);
  assert(!is_null($R));
  foreach ( $R as $r ) {
    $id   = $r['id'];
    $name = $r['name'];
    $X[$name] = $id;
    $reverse_X[$id] = $name;
  }
  $GLOBALS[$tbl] = $X;
  $GLOBALS["reverse_" .. $tbl] = $reverse_X;
}
//------------------------------------------
function load_globals()
{
  make_lkp("test_type");
  make_lkp("state");
  make_lkp("bin_type");
  make_lkp("admin");
}
?>

<?php
require_once 'bye.php';
require_once 'db_get_rows.php';
function swap($tbl)
{

  $R = db_get_rows($tbl);
  assert(!is_null($R));
  foreach ( $R as $r ) {
    $id   = $r['id'];
    $name = $r['name'];
    $X[$name] = $id;
  }
  $GLOBALS[$tbl] = $X;
}
//------------------------------------------
function load_globals()
{
  swap("test_type");
  swap("state");
  swap("bin_type");
  swap("admin");
}
?>

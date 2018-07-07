<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "lkp.php";
require_once "rs_assert.php";

function db_get_filters(
)
{
  $AT = db_get_rows("attr_type");
  foreach ( $AT as $at ) { 
    $attr_type    = $at['name'];
    $attr_type_id = $at['id'];
    unset($X);
    $X = array(); $xidx = 0;
    $A = db_get_rows("attr", " attr_type_id = $attr_type_id ");
    foreach ( $A as $a ) { 
      $X[$xidx++] = $a['name'];
    }
    // print("attr_type = $attr_type\n");
    $out[$attr_type] = $X;
  }
  return $out;
}
$x = db_get_filters();
var_dump(json_encode($x));
?>

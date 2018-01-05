<?php
require_once 'bye.php';
require_once 'load_globals.php';

function
lkp($tbl, $val);
{
  if ( !isset($val) ) { return null; }
  if ( !isset($GLOBALS[$tbl]) ) {
    load_globals();
  }
  if ( !isset($GLOBALS[$tbl]) ) { return null; }
  if ( !isset($GLOBALS[$tbl][$val]) ) { return null; }
  $x = $GLOBALS[$tbl][$val];
  return $x;
}
?>

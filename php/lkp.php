<?php
require_once 'bye.php';
require_once 'load_globals.php';
require_once 'load_configs.php';

function
lkp($tbl, $val)
{
  if ( !isset($tbl) ) { assert(null, "Table $tbl not specified"); }
  if ( !isset($val) ) { assert(null, "Value $val not specified"); }
  if ( !isset($GLOBALS[$tbl]) ) {
  if ( strtolower($tbl) == "configs" ) {
    load_configs();
  }
  else {
    load_globals();
  }
  }
  assert(isset($GLOBALS[$tbl]));
  if (! isset($GLOBALS[$tbl][$val]) ) {
    echo("UNDEFINED $tbl, $val\n");
  }

  assert(isset($GLOBALS[$tbl][$val]));
  $x = $GLOBALS[$tbl][$val];
  return $x;
}
?>

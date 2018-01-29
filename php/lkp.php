<?php
require_once 'load_globals.php';
require_once 'load_configs.php';
require_once 'rs_assert.php';

function
lkp(
  $tbl, 
  $val,
  $reverse = false
)
{
  if ( $reverse ) { 
    $tbl = "reverse_" . $tbl; 
  }
  if ( !isset($tbl) ) { 
    rs_assert(null, "Table $tbl not specified"); 
  }
  if ( !isset($val) ) { 
    rs_assert(null, "Value $val not specified"); 
  }
  if ( !isset($GLOBALS[$tbl]) ) {
    if ( strtolower($tbl) == "configs" ) {
      load_configs();
    }
    else {
      load_globals();
    }
  }
  rs_assert(isset($GLOBALS[$tbl]));
  if (! isset($GLOBALS[$tbl][$val]) ) {
    echo("UNDEFINED $tbl, $val\n");
  }
  rs_assert(isset($GLOBALS[$tbl][$val]));
  $x = $GLOBALS[$tbl][$val];
  return $x;
}
?>

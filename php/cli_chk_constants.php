<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
require_once "chk_constants.php";
$GLOBALS['err'] = "";
$x = chk_constants();
if ( !$x ) { 
  echo "ERROR]n";
  var_dump($GLOBALS['err']);
}
else {
  echo "Constants match up\n";
}
?>

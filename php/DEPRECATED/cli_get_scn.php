<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "get_scn.php";

$x = get_scn();
if ( is_null($x) ) {
  echo "ERROR:\n";
  var_dump($GLOBALS['err']);
  exit(1);
}
else {
  echo " { \"SCN\" : \"$x\" } \n";
  exit(0);
}
?>

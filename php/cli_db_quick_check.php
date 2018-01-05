<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "db_quick_check.php";

$x = db_quick_check();
if ( !$x ) { 
  echo "Database Integrity Error\n";
  var_dump($GLOBALS['err']);
  exit(1);
}
else {
  echo "ALL WELL \n";
  exit(0);
}
?>

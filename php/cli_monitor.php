<?php
require_once "monitor.php";
$GLOBALS["err"] = "";
if ( monitor() ) {
  echo "{ \"STATUS\" : \"OK\" }";
}
else {
  echo "{ \"STATUS\" : \"ERROR\", \"DETAILS\" : \"{$GLOBALS['err']}\" }";
}

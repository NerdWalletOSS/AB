<?php
require_once "chk_rts.php";
$GLOBALS["err"] = "";
if ( chk_rts() ) {
  echo "ALL IS WELL \n";
  exit(0);
}
else {
  echo "ERROR\n";
  var_dump($GLOBALS['err']);
  exit(1);
}
?>

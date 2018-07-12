<?php
function go_BYE($msg ) {
  if ( ( !is_null($msg) ) && ( $msg != "" ) ) {
    $GLOBALS["err"] = $msg;
    echo $msg . "\n"; 
  }
  debug_print_backtrace();
  exit(1); 
}
?>

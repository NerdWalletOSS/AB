<?php
function rs_assert(
  $val, 
  $err, 
  $code = 400
)
{
  if ( empty($val) ) {
    debug_print_backtrace();
    header("Error-Message: " . nl2br($err));
    header("Error-Code: " . $code);
    ob_flush();
    exit(1);
  }
}

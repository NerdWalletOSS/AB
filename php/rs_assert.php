<?php
function rs_assert(
  $val, 
  $err, 
  $code = 400
)
{
  if ( empty($val) ) {
    $x = json_encode(debug_backtrace());
    header("Error-BackTrace: " . nl2br($x));
    header("Error-Message: " . nl2br($err));
    header("Error-Code: " . $code);
    ob_flush();
    exit(1);
  }
}

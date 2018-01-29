<?php
function rs_assert(
  $val, 
  $err = "GENERIC_ERROR", 
  $code = 400
)
{
  if ( empty($val) ) {
    if ( getenv("AB_PHP_CLI") ) { 
      $X['backtrace'] = debug_backtrace();
      $X['stderr']    = $err;
      $X['code']      = $code;
      print_r(json_encode($X));
    }
    else {
      $x = json_encode(debug_backtrace());
      header("Error-BackTrace: " . nl2br($x));
      header("Error-Message: " . nl2br($err));
      header("Error-Code: " . $code);
      http_response_code($code);

      ob_flush();
    }
    exit(1);
  }
}
?>

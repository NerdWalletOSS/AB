<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'db_set_row.php';
function rs_assert(
  $val, 
  $err = "GENERIC_ERROR", 
  $code = 400
)
{
  if ( empty($val) ) {
    $Y['msg_stderr'] = $err;
    $Y['status_code'] = $code;
    if ( isset($_SESSION['REQUEST_WEBAPP_ID']) ) {
      $request_webapp_id = $_SESSION['REQUEST_WEBAPP_ID'];
      if ( is_int($request_webapp_id) && ( $request_webapp_id > 0 ) ) { 
        db_set_row("log_ui_to_webapp", $request_webapp_id, $Y);
      }
    }
    if ( getenv("AB_PHP_CLI") ) { 
      $X['backtrace'] = debug_backtrace();
      $X['stderr']    = $err;
      $X['code']      = $code;
      print_r(json_encode($X));
    }
    else {
      $x = json_encode(debug_backtrace());
      header("Error-BackTrace: " . nl2br($x));
      $err = str_replace("\n", "__", $err);
      header("Error-Message: " . nl2br($err));
      header("Error-Code: " . $code);
      http_response_code($code);

      //ob_flush();
    }
    exit(1);
  }
}
?>

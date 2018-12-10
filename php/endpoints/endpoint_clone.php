<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'test_clone.php';
//-----------------------------------------------------------
ob_start();
$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  $err = '{ "Clone" : "ERROR", "Message" : "No payload" }'; 
  header("Error-Message: " . nl2br($err));
  header("Error-Code: 400");
  http_response_code(400);
  exit;
}
$rslts =  test_clone($body);
if ( is_array($rslts ) ) { 
  foreach ( $rslts as $key=> $val ) {
    $val = str_replace("\n", " ", $val);
    header("$key: $val");
  }
}
ob_clean();
?>

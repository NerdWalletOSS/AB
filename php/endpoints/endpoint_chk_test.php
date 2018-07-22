<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'chk_test.php';
//-----------------------------------------------------------

$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  $err = '{ "ChkTest" : "ERROR", "Message" : "No payload" }'; 
  header("Error-Message: " . nl2br($err));
  header("Error-Code: 400");
  http_response_code(400);
  exit;
}
$rslt =  chk_test($body);
if ( !$rslt ) { 
  header("Error-Code: 400"); 
  echo '{ "ChkTest" : "ERROR", "Message" : "test failed " }'; exit;
}
?>

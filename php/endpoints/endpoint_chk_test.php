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
  echo '{ "ChkTest" : "ERROR", "Message" : "No payload" }'; exit;
}
$rslt =  chk_test($body);
foreach ( $rslt as $key=> $val ) {
  header("$key: $val");
}
?>

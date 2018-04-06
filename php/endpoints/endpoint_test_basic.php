<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
// set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
// set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");

require_once 'test_basic.php';
//-----------------------------------------------------------
ob_start();
$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No payload" }'; exit;
}
$rslt =  test_basic($body);
//var_dump($rslt);
header("TestID: ".$rslt["TestID"]);
ob_clean();
?>

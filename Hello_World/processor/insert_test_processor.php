<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'insert_test.php';
/* Sample usage
X='{ "Creator" : "joe", "Name" : "Test3", "Description" : "Test Desciption", "TestType" : "ABTest", "NumVariants" : "3", "Variants" : [ { "Name" : "Control", "Description" : "Control", "Percentage" : "85" }, { "Name" : "Red", "Description" : "Red Desc", "Percentage" : "10" }, { "Name" : "Blue", "Description" : "Blue Desc", "Percentage" : "5" } ] }'
curl -v --url "localhost:8080/AB/tcc/json_set_ramp_process.php" --data-urlencode "INPUT=$X"
 */

//-----------------------------------------------------------
//-------- ACCESS POST parameters
//$X = json_decode(file_get_contents('php://input'), true);
ob_start();
if ( !$_POST ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$json_input = json_encode($_POST);
if ( !$json_input ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "Bad JSOn" }'; exit;
}
//-------------------------------------

$rslt =  insert_test($json_input);
ob_clean();

?>

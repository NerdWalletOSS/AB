<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'test_basic.php';
require_once '../create_good_json_test.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
}
var_dump($_POST);
$str_inJ = json_encode($_POST);
$json_input = create_good_json_test($str_inJ);
if ( !$json_input ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "Bad JSOn" }'; exit;
}
 //print("<pre>".print_r($json_input,true)."</pre>");
//-------------------------------------
// Call to add test
$rslt =  test_basic($json_input);
//var_dump($rslt);
header("TestID: ".$rslt["TestID"]);
ob_clean();

?>

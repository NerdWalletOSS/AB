<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'db_get_test.php';
require_once 'set_device_specific_variant.php';
require_once "get_variant_idx.php";
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
}
var_dump($_POST); exit();
$test_id = $_POST['TestID'];
$TestType = $_POST['TestType'];
//$i = $_POST['Position'];
$X = db_get_test($test_id);

print("<pre>".print_r($X,true)."</pre>");
//-------------------------------------
// Call to Add/Edit Additional Variant Information
//$rslt =  set_device_specific_variant($json_decode($X)); //add_addnl_var_info(json_encode($X));
//var_dump($rslt);
header("TestID: ".$id);
//header("TestID: ".$rslt["TestID"]);
ob_clean();
?>

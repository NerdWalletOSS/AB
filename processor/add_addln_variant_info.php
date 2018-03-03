<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'db_get_test.php';
require_once 'add_addnl_var_info.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
}
var_dump($_POST);
$test_id = $_POST['TestID'];
$i = $_POST['Position'];
$X = db_get_test($test_id);
$X['Updater'] = $_POST['Updater'];
$X['Variants'][$i]['description'] = $_POST['Description'];
if ( isset($TestType) && ($TestType == "XYTest")) {
$X['Variants'][$i]['custom_data'] = $_POST['CustomData'];
}
$X['VariantID'] = $_POST['VariantID'];
//-------------------------------------
// Call to Add/Edit Additional Variant Information
$rslt =  add_addnl_var_info(json_encode($X));
//var_dump($rslt);
header("TestID: ".$rslt["TestID"]);
ob_clean();
?>

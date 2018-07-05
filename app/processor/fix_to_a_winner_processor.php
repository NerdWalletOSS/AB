<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'db_get_test.php';
require_once 'set_state.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Fix to a Winner" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$test_id = $_POST['TestID'];
// SETTING Winner
$X = db_get_test($test_id);
$X['NewState'] = "terminated";
$X['Winner'] = $_POST['Winner'];
$X['Updater'] = $_POST['Updater'];
$str_inJ = json_encode($X);
//-------------------------------------
// Call to Add/Edit Additional Variant Information
//print("<pre>".print_r($T,true)."</pre>");
$outJ = set_state($str_inJ); //set_state(json_encode($X));
header("TestID: ".$test_id);
ob_clean();
?>

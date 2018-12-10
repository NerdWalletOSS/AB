<?php
session_start();
$TestType = $_SESSION['TestType'];
$user = $_SESSION['User'];
# -- CALL REQUIRED FILES 
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
# -- Get Active Tests
require_once "db_get_rows.php";
//if ( !$_POST ) {
//  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
//}
//var_dump(json_decode($_POST));
//$admin = $data['admin'];
//$option = $data['option'];
$admin = $_GET['admin'];
$option = $_GET['option'];
switch ($option) {
    case "2":
        $state = "'1', '2'";
        break;
    case "3":
        $state = '5';
        break;
    default:
        $state = "'3','4'";
}
if (isset($TestType)) {
if ($TestType == "ABTest") { $test_type_id = 1; }
if ($TestType == "XYTest") { $test_type_id = 2; }
if ((isset($admin)) && ($admin != "") ) {
$result = db_get_rows("test", "test_type_id = ".$test_type_id." and state_id IN (".$state.") and creator_id = '".$admin."' order by updated_at DESC");
} elseif ($admin == "") {
$result = db_get_rows("test", "test_type_id = ".$test_type_id." and state_id IN (".$state.") order by updated_at DESC");
} else {
echo "test";
// Do NOTHING
}
echo json_encode($result);
}

?>

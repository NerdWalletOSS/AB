<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");

require_once "php/test_clone.php";
//-----------------------------------------------------------
$old_test_id = $_GET['id'];
$creator = $_GET['creator'];
$clone_name = $_GET['clone'];

$in['OldTestID'] = $old_test_id;
$in['Creator'] = $creator;
$in['NewTestName'] = $clone_name;
$str_inJ = json_encode($in);
$x = test_clone($str_inJ);
if ($x != false ) {
echo json_encode(array('message' => 'Test Name '.$old_test_name.' cloned as '.$clone_name));
} else {
echo json_encode(array('message' => 'Failed to clone'));
}
?>

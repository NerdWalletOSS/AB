<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");


//-----------------------------------------------------------
$old_test_id = $_GET['id'];
$old_test_name = $_GET['name'];
$clone_name = $_GET['clone'];
echo json_encode(array('message' => 'You wish to clone Test Name '.$old_test_name.' with id '.$old_test_id.' cloned as '.$clone_name));

?>

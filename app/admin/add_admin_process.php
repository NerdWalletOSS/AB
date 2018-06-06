<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "add_admin_channel.php";

//-----------------------------------------------------------
$admin_name = $_GET['admin_name'];
$X = array();
$X['Table'] = 'admin';
$X['Name'] = $admin_name;
$str_inJ = json_encode($X);
$rslt = add_admin_channel($str_inJ);
if ( !$rslt ) { 
  header('Location: list_admin.php?msg='.urlencode($GLOBALS["err"]));
}
else {
  header('Location: list_admin.php?msg=Admin Name Added');
}
?>

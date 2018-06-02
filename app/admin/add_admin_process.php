<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "add_admin.php";


//-----------------------------------------------------------
$admin_name = $_GET['admin_name'];
//$uri       = $_GET['uri'];

$rslt = add_admin($admin_name);
if ( !$rslt ) { 
  header('Location: admin.php?msg='.urlencode($GLOBALS["err"]));
}
else {
  header('Location: admin.php?msg=Admin Name Added');
}
?>

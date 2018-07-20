<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/db_helpers/");

require_once "chk_tests.php";

//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Check Tests" : "ERROR", "Message" : "No paylaod" }'; exit;
}

//-------------------------------------
// Call to check tests
$rslt =  chk_tests();
ob_clean();
?>

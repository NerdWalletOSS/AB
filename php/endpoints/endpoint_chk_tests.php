<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'chk_tests.php';
//-----------------------------------------------------------
$rslt =  chk_tests();
if ( !$rslt ) { 
  header("Error-Code: 400"); 
  echo '{ "ChkTests" : "ERROR", "Message" : "test failed " }'; exit;
}
?>

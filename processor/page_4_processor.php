<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'db_get_test.php';
require_once 'set_follow_on.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Follow On" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$tid = $_POST['TestID'];
$tid_to_follow = $_POST['tid_to_follow'];
//-------------------------------------
// Set Follow On
$rslt =  set_follow_on($tid, $tid_to_follow);
ob_clean();
?>

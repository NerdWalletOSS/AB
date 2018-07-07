<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../../php/db_helpers/");

require_once "add_admin_channel.php";

//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Add Channel" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$channel = $_POST['channel'];
$X = array();
$X['Table'] = 'channel';
$X['Name'] = $channel;
$str_inJ = json_encode($X);
if ( !$str_inJ ) {
  echo '{ "Add Channel" : "ERROR", "Message" : "Bad JSOn" }'; exit;
}
//-------------------------------------
// Call to add admin
$rslt =  add_admin_channel($str_inJ);
ob_clean();
?>

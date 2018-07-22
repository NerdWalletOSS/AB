<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'add_admin_channel.php';
//-----------------------------------------------------------

$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  echo '{ "AddAdminChannel" : "ERROR", "Message" : "No payload" }'; exit;
}
$rslts =  add_admin_channel($body);
if ( is_array($rslts ) ) { 
  foreach ( $rslts as $key=> $val ) {
    $val = str_replace("\n", " ", $val);
    header("$key: $val");
  }
}
?>

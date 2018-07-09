<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'set_state.php';
//-----------------------------------------------------------
ob_start();
$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  echo '{ "SetState" : "ERROR", "Message" : "No payload" }'; exit;
}
$rslts =  set_state($body);
if ( is_array($rslts ) ) { 

  foreach ( $rslts as $key=> $val ) {
    // $val = convert_uuencode($val);
    $val = str_replace("\n", " ", $val);
    header("$key: $val");
  }

}
ob_clean();
?>

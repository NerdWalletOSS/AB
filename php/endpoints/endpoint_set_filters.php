<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'set_filters.php';
//-----------------------------------------------------------

$body = file_get_contents('php://input');
if ( ( empty($body) ) || ( trim($body) == "" ) ) {
  echo '{ "SetFilters" : "ERROR", "Message" : "No payload" }'; exit;
}

$body = file_get_contents("/tmp/_x.json");
$rslt =  set_filters($body);
foreach ( $rslt as $key=> $val ) {
  header("$key: $val");
}
?>

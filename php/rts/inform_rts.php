<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");

require_once 'rs_assert.php';
require_once 'list_rts.php';
require_once 'post_url.php';
require_once 'db_get_test.php';

function inform_rts(
  $test_id,
  &$err_msg,
  $is_check = false
)
{

  $err_msg = "";
  $is_ok = true;
  $SP = list_rts();
  if ( is_null($SP) ) { 
    $err_msg = "NOTICE: Not talking to RTS\n";
    return true;  
  } 
  foreach ( $SP  as $sp ) {
    $rslt = ""; $http_code = 0;
    $server = $sp['server']; $port   = $sp['port'];
    $body = json_encode(db_get_test($test_id));
    $url = "AddTest";
    if ( $is_check ) {
      $url = "CheckTest";
    }
    post_url($server, $port, $url, $body, $http_code, $rslt);
    if ( $http_code != 200 ) { 
			$err_msg .= $rslt; $is_ok = false; }
  }
  return $is_ok;
}
/*
$err = "";
$x = inform_rts(1, $err);
var_dump($x);
var_dump($err);
 */
?>

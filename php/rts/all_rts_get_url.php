<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "list_rts.php";
require_once "get_url.php";

function all_rts_get_url(
  $url
)
{
  $SP = list_rts();
  $first = true;
  $rval = array(count($SP)); $ridx = 0;
  foreach ( $SP as $sp ) { 
    $server = $sp['server'];
    $port   = $sp['prot'];
    $http_code = 0; $rslt = "";
    $status = get_url($server, $port, $url, $http_code, $rslt);
    rs_assert($status); 
    $rval[$ridx++] = $rslt;
  }
  return $rval;
}

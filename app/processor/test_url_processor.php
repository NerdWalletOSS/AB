<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "get_url.php";

//-----------------------------------------------------------
$nw_campaign_id = $_POST['nw_campaign_id'];
$device = $_POST['device'];
if (isset($device) && ( $device != "")) {
  $url = "Router?nw_campaign_id=".$nw_campaign_id."&Device=".$device;
} else {
  $url = "Router?nw_campaign_id=".$nw_campaign_id;
}

$http_code = 0;
$rslt = "";
$destination = "";
$data = get_url( 'localhost', '8000',$url, $http_code, $rslt, $destination );
header("URLReturned: ".$destination);
header("StatusCodeReturned: ".$http_code);
?>

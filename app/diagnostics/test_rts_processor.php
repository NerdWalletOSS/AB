<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'get_url.php';
//-----------------------------------------------------------
//-------- ACCESS GET parameters
$server = $_GET['server'];
$port = $_GET['port'];
// Call to check rts
$url = 'Ignore';
$http_code = 0;
$rslt = "";
$data = get_url( $server, $port, $url, $http_code, $rslt );
if ($http_code == 200 ) {
  $_SESSION['test_status'] = "<span style='color:green;font-weight:bold'>SUCCESS: RTS with server as ".$server." and ".$port." is in GOOD STATE</span>";
}
else {
  $_SESSION['test_status'] = "<span style='color:red;font-weight:bold'>ERROR: Something Wrong with RTS having server as ".$server." and port as ".$port."</span>";
}



// CASE: FIX TO A WINNER
/*
$X['NewState'] = action_state($_GET['state_id']);
if ((isset($_GET['Winner'])) && ($_GET['state_id'] == "2")) {
  $X['Winner'] = $_GET['Winner'];
}
*/


header('Location: list_rts.php');

?>

<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'get_url.php';
//-----------------------------------------------------------
//-------- ACCESS GET parameters
$nw_campaign_id = $_GET['nw_campaign_id'];
$TestID = $_GET['TestID'];
// Call to check rts
$url = 'Router?nw_campaign_id='.$nw_campaign_id;
$http_code = 0;
$rslt = "";
$data = get_url( 'localhost', '8000', $url, $http_code, $rslt );
if ($http_code == 200 ) {
  $_SESSION['campaign_status'] = "<span style='color:green;font-weight:bold'>SUCCESS: Campaign is in GOOD STATE</span>";
}
else {
  $_SESSION['campaign_status'] = "<span style='color:red;font-weight:bold'>ERROR: Something wrong with Campaign URL</span>";
}



// CASE: FIX TO A WINNER
/*
$X['NewState'] = action_state($_GET['state_id']);
if ((isset($_GET['Winner'])) && ($_GET['state_id'] == "2")) {
  $X['Winner'] = $_GET['Winner'];
}
*/


header('Location: ../aev_test_1.php?TestID='.$TestID);

?>

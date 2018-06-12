<?php session_start();
# -- PARSE SESSION VARIABLES
if (isset($_SESSION['User'])) { $User = $_SESSION['User'];}
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'chk_test.php';
//-----------------------------------------------------------
//-------- ACCESS SESSION VARIABLE
if (isset($_SESSION['TestType']))
	{
		$TestType = $_SESSION['TestType'];
	}
else
	{
		header('Location: index.php?error=Test Type not set FILE: ' . __FILE__ . ' :LINE: ' . __LINE__ . '');
		return false;
	}

// Call to check test
$TestName = $_GET['TestName'];
$X = array();
$X['TestName'] = $TestName;
$X['TestType'] = $TestType;
$str_inJ = json_encode($X);
$status = chk_test($str_inJ);

if ($status == 1 ) {
  $_SESSION['test_status'] = "<span style='color:green;font-weight:bold'>SUCCESS: ".$TestName." is in good state with respect to RTS & DB </span>";
}
else {
  $_SESSION['test_status'] = "<span style='color:red;font-weight:bold'>ERROR: ".$TestName." is not in sync with respect to RTS & DB</span>";
}

// CASE: FIX TO A WINNER
/*
$X['NewState'] = action_state($_GET['state_id']);
if ((isset($_GET['Winner'])) && ($_GET['state_id'] == "2")) {
  $X['Winner'] = $_GET['Winner'];
}
*/


header('Location: ../home.php');

?>

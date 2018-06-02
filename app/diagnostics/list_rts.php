<?php //session_start(); 
//if (isset($_SESSION['User']) && isset($_SESSION['TestType'])) {header('Location: home.php');} else { /* Do Nothing */}
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php");

require_once "../../php/rts/list_rts.php";

/*
function all_rts_get_url(
  $url
)
{
  $SP = list_rts();
  if ((count($SP)) > 0 ) {
  $first = true;
  $rval = array(count($SP)); $ridx = 0;
  foreach ( $SP as $sp ) { 
    $server = $sp['server'];
    $port   = $sp['port'];
    $http_code = 0; $rslt = "";
    $status = get_url($server, $port, $url, $http_code, $rslt);
    rs_assert($status); 
    $rval[$ridx++] = $rslt;
  }
  return $rval;
  } else {
  return $rval = 0;
  }
}
*/
require_once "../includes/header_diagnostics.php"; 
?>

<div class="container theme-showcase" role="main" >
<?php 
//if ((isset($_GET['service'])) && ($_GET['service'] != "")) {
//$service = $_GET['service'];
//$url = "localhost:8000/PingServer?Service=".$service;
//$url = "localhost:8000/";
//$rval = all_rts_get_url($url);
//var_dump($rval);
//if ((!rval) || ($rval == 0)) {  echo "<strong>NO TEST REGISTERED IN RTS</strong>";}
//}
$SP = list_rts();
if ( $SP ) { 
   foreach ( $SP as $sp ) { 
     $server = $sp['server']; $port = $sp['port'];
     echo "server = $server, port = $port \n";
  }
}
else {
  echo "Not using RTS\n";
}
?> 
</div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

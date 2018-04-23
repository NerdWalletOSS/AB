<?php session_start(); 
if (isset($_SESSION['User']) && isset($_SESSION['TestType'])) {header('Location: home.php');} else { /* Do Nothing */}

set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php");
require_once "rs_assert.php";
require_once "list_rts.php";
require_once "get_url.php";
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
  echo "<strong>NO TEST REGISTERED IN RTS</strong>";
  }
}
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="Assures Quality with business logic">
    <meta name="author" content="Nerdwallet QA Team">
    <link rel="icon" href="images/favicon.ico">

    <title>AB Diagnostics Center</title>

    <!-- Bootstrap core CSS -->
    <link href="bootstrap-3.3.5-dist/css/bootstrap.min.css" rel="stylesheet">
    <!-- Custom styles for this template -->
    <link href="css/theme.css" rel="stylesheet">
  </head>

  <body>
<!-- Fixed navbar -->
	<nav class="navbar navbar-inverse navbar-fixed-top">
	<div class="container">
	<div class="navbar-header">
<!-- Application Title -->
	<a class="navbar-brand" id="modLink" href="diagnostics.php">AB/URL Diagnostics</a>
	</div>
 <div class="navbar-collapse collapse" id="navbar">
      <ul class="nav navbar-nav navbar-right">
          <li>
          <a href="diagnostics.php?service=logger"><button class="btn btn-sm btn-info" type="button">Logger</button></a>
        </li>        
         <li>
          <a href="diagnostics.php?service=ss"><button class="btn btn-sm btn-info" type="button">SS</button></a>
        </li>
        <li>
          <a href="diagnostics.php?service=webapp"><button class="btn btn-sm btn-info" type="button">Webapp</button></a>
        </li>
      </ul>
    </div><!--/.nav-collapse -->
  </div>
  </nav>
<div class="container theme-showcase" role="main" >
<?php if ((isset($_GET['service'])) && ($_GET['service'] != "")) {
$service = $_GET['service'];
$url = "localhost:8000/PingServer?Service=".$service;
$rval = all_rts_get_url($url);
}
?> 
</div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "includes/footer.php"; ?>

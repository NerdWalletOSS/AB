<?php session_start(); 
if (isset($_SESSION['User']) && isset($_SESSION['TestType'])) {header('Location: home.php');} else { /* Do Nothing */}
// -- SET PATH
require_once "set_path.php";
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
	<a class="navbar-brand" id="modLink" href="#">AB/URL Diagnostics</a>
	</div>
 <div class="navbar-collapse collapse" id="navbar">
      <ul class="nav navbar-nav navbar-right">
          <li>
          <a href="diagnostics_processor.php?service=logger"><button class="btn btn-sm btn-danger" type="button">Logger</button></a>
        </li>        
         <li>
          <a href="diagnostics_processor.php?service=ss"><button class="btn btn-sm btn-danger" type="button">SS</button></a>
        </li>
        <li>
          <a href="diagnostics_processor.php?service=webapp"><button class="btn btn-sm btn-danger" type="button">Webapp</button></a>
        </li>
      </ul>
    </div><!--/.nav-collapse -->
  </div>
  </nav>
<div class="container theme-showcase" role="main" >

</div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "footer.php"; ?>

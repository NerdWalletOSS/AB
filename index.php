<?php session_start(); 
if (isset($_SESSION['User']) && isset($_SESSION['TestType'])) {header('Location: home.php');} else { /* Do Nothing */}
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

    <title>Nerdwallet Testing Control Center</title>

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
	<a class="navbar-brand" id="modLink" href="#">AB/URL Test Manager</a>
	</div>
  </div>
  </nav>
<div class="container theme-showcase" role="main" >
		<div class="row" >
	        <div class="col-xs-6 col-xs-offset-3">
		  <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Please Login</h3>
            </div>
            <div class="panel-body">
	<form class="form-signin" action="login_process.php">
	<!-- Error Div to display error, if exist -->
	<div class="error"><?php if (isset($_GET['error']) ) { echo $_GET['error'];} else {/*Do nothing */} ?></div>
        <label for="user" class="sr-only">User Name</label>
        <input name="User"  type="text" id="user" class="form-control" placeholder="User name" required autofocus>
        <div class="radio">
        <label><input type="radio" name="TestType" value='ABTest' checked>A/B Test Manager</label>
        </div>
        <div class="radio">
         <label><input type="radio" name="TestType" value='XYTest'>URL Router</label>
         </div>
        <button class="btn btn-lg btn-primary btn-block" type="submit">Sign in</button>
        </form>
    </div></div></div></div>
</div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

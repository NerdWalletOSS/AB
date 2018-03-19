<?php
session_start();

// -- ERROR REPORTING
error_reporting(E_ALL);
ini_set('display_errors', '1');

// -- TEST SESSION CREDENTIALS
if ((!isset($_SESSION['User'])) || (!isset($_SESSION['TestType'])))
	{
		header('Location: index.php?error=Login Please!');
		return false;
	}

// -- PARSE SESSION VARIABLES
if (isset($_SESSION['User']))
	{
		$User = $_SESSION['User'];
	}
else
	{
		header('Location: index.php?error=User name not set FILE: ' . __FILE__ . ' :LINE: ' . __LINE__ . '');
		return false;
	}

if (isset($_SESSION['TestType']))
	{
		$TestType = $_SESSION['TestType'];
	}
else
	{
		header('Location: index.php?error=Test Type not set FILE: ' . __FILE__ . ' :LINE: ' . __LINE__ . '');
		return false;
	}
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="Assures Quality with business logic">
    <meta name="author" content="Nerdwallet QA Team">
    <link rel="icon" href="images/favicon.ico">
     <title>
<?php
if ($TestType == "ABTest")
	{
		echo "A/B Test Manager";
	}
elseif ($TestType == "XYTest")
	{
		echo "URL Router";
	}
else
	{
		echo "SOMETHING WRONG";
	} 
?>
    </title>

    <!-- Bootstrap core CSS -->
    <link href="bootstrap-3.3.5-dist/css/bootstrap.min.css" rel="stylesheet">
    <!-- Bootstrap theme -->
    <link href="bootstrap-3.3.5-dist/css/bootstrap-theme.min.css" rel="stylesheet">
    <!-- Custom styles for this template -->
    <link href="css/theme.css" rel="stylesheet">
    <link href="css/error.css" rel="stylesheet">
    
    <!-- jQuery -->
    <script src="js/jquery-1.11.1.min.js"></script>
    <script src="js/bootstrap.min.js"></script>

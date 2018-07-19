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

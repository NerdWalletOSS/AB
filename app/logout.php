<?php
session_start();  
session_destroy(); // Destroy session variables.  
header("Location: index.php");// Redirect to the index page.
?>

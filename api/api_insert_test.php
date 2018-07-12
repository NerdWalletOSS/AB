<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'insert_test.php';

$x = $_POST;
var_dump($x);
?>

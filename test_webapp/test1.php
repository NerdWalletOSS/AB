<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once 'insert_test.php';


$infile = "good_insert_1.json";

assert(is_file($infile));
$str_inJ = file_get_contents($infile);
$outJ = insert_test($str_inJ);
$str_outJ = json_encode($outJ);
assert(isset($str_outJ));
?>

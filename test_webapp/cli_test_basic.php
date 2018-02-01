<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers");

require_once 'rs_assert.php';
require_once 'insert_test.php';
require_once 'db_get_test.php';

if ( $argc != 2 ) { echo "Expected 3 arguments. Got $argc"; exit(1); }

$infile = $argv[1];

assert(is_file($infile));
// Create a test using information from file
$str_inJ = file_get_contents($infile);
$outJ = insert_test($str_inJ);
//  echo(json_encode($outJ));
//- Get testID of created file and try to change description 
$test_id = $outJ['TestID'];
$X = db_get_test($test_id);
$X['description'] = "New Description";
$X['Updater'] = $X['Creator'];
$str_inJ = json_encode($X);
$outJ = insert_test($str_inJ);
 echo(json_encode($outJ));


?>

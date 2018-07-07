<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");

require_once 'rs_assert.php';
require_once 'db_get_test.php';

if ( $argc != 2 ) { echo "Expected 1 argument. Got $argc"; exit(1); }

$test_id = $argv[1];
$X = db_get_test($test_id);
if ( empty($X) ) { 
  echo "{ } "; 
}
else {
  echo json_encode($X);
}
?>

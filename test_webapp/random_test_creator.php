<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers");

require_once 'rs_assert.php';
require_once 'test_basic.php';
require_once 'db_get_test.php';
require_once 'set_state.php';

function archive($test_id) {
  // archive the test
  $X = db_get_test($test_id);
  $X['NewState'] = "archived";
  $X['Updater'] = $X['Creator'];
  $str_inJ = json_encode($X);
  $outJ = set_state($str_inJ);
}

function terminate($test_id, $winner_idx = 0) {
  // terminate the test
  $X = db_get_test($test_id);
  $X['NewState'] = "terminated";
  $X['Winner'] = $X['Variants'][$winner_idx]['name'];
  $X['Updater'] = $X['Creator'];
  $str_inJ = json_encode($X);
  $outJ = set_state($str_inJ);
}

function start($test_id) {
  // start the test
  $X = db_get_test($test_id);
  $X['NewState'] = "started";
  $X['Updater'] = $X['Creator'];
  $str_inJ = json_encode($X);
  $outJ = set_state($str_inJ);
}

function publish($test_id) {
  // Publish the test
  $X = db_get_test($test_id);
  $X['NewState'] = "dormant";
  $X['Updater'] = $X['Creator'];
  $str_inJ = json_encode($X);
  $outJ = set_state($str_inJ);
}

if ( $argc != 2 ) { echo "Expected 3 arguments. Got $argc"; exit(1); }

$infile = $argv[1];

assert(is_file($infile));
// Create a test using information from file
$data = file_get_contents($infile);
for ($i = 1; $i <= 10; $i++) {
  $X = json_decode($data);
  if ( $i % 3 == 0) {
  $X->{'TestType'} = "ABTest";
  }
  $test_name = "T".$i;
  $X->{'name'} = $test_name;
  var_dump($X);
  $outJ = test_basic(json_encode($X));
  //- Get testID of created test
  $test_id = $outJ['TestID'];
  echo "Created test with test id ".$test_id;
  publish($test_id);
  start($test_id);
  if ( $i % 2 == 0) {
    terminate($test_id);
    archive($test_id);
  } else {
  // Do Nothing;
  }
}
exec('mysqldump --user=root --host=localhost abdb2 > mysqldump.sql');

echo "COMPLETED";
?>

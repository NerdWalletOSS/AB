<?php
error_reporting(E_ALL);
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers");

require_once 'rs_assert.php';
require_once 'test_basic.php';
require_once 'db_get_test.php';
require_once 'set_state.php';
require_once 'add_addnl_var_info.php';
require_once 'find_tests_to_follow.php';
require_once 'set_follow_on.php';

function archive($test_id) {
  // archive the test
  $X = db_get_test($test_id);
  $X['NewState'] = "archived";
  $X['Updater'] = $X['Creator'];
  $str_inJ = json_encode($X);
  $outJ = set_state($str_inJ);
}

function terminate($test_id) {
  // terminate the test
  $X = db_get_test($test_id);
  $X['NewState'] = "terminated";
  $X['Winner'] = $X['Variants'][1]['name'];
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
$str_inJ = file_get_contents($infile);
$outJ = test_basic($str_inJ);
//- Get testID of created file and try to change description 
$test_id = $outJ['TestID'];
$X = db_get_test($test_id);
$X['description'] = "New Description";
$X['Updater'] = $X['Creator'];
$str_inJ = json_encode($X);
$outJ = test_basic($str_inJ);
publish($test_id);
// Update the custom data and description
$X = db_get_test($test_id);
$X['Updater'] = $X['Creator'];
$X['Variants'][0]['custom_data'] = " {} ";
$X['Variants'][0]['description'] = " foo bar";
$X['VariantID'] = $X['Variants'][0]['id'];
$outJ = add_addnl_var_info(json_encode($X));
// Update the percentages
$X = db_get_test($test_id);
$X['Updater'] = $X['Creator'];
$V = $X['Variants'];
$vidx = 0;
foreach ( $V as $v ) { 
  if ( $vidx == 0 ) { 
    $X['Variants'][$vidx]['percentage'] = 100;
  }
  else {
    $X['Variants'][$vidx]['percentage'] = 0;
  }
  $vidx++;
}
$outJ = test_basic(json_encode($X));
terminate($test_id);
archive($test_id);
// create new test with same name 
$str_inJ = file_get_contents($infile);
$outJ = test_basic($str_inJ);
$test_id = $outJ['TestID'];
$X = db_get_test($test_id);
$Channel = $X['Channel'];
$F = find_tests_to_follow($Channel);
$tid_to_follow = $F[0]['id'];
set_follow_on($test_id, $tid_to_follow);
//-- Create T2 and archive it 
$str_inJ = file_get_contents($infile);
$T = json_decode($str_inJ);
$T->{'name'} = 'T2';
$outJ = test_basic(json_encode($T));
$test_id = $outJ['TestID'];
publish($test_id);
start($test_id);
terminate($test_id);
archive($test_id);
//-- Create T3  and publish it 
$str_inJ = file_get_contents($infile);
$T = json_decode($str_inJ);
$T->{'name'} = 'T3';
$outJ = test_basic(json_encode($T));
$test_id = $outJ['TestID'];
publish($test_id);
//-- Create T4  and leave it dormant 
$str_inJ = file_get_contents($infile);
$T = json_decode($str_inJ);
$T->{'name'} = 'T4';
$outJ = test_basic(json_encode($T));
$test_id = $outJ['TestID'];
echo "COMPLETED";
?>

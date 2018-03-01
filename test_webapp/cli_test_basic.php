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
// Publish the test
$X = db_get_test($test_id);
$X['NewState'] = "dormant";
$X['Updater'] = $X['Creator'];
$str_inJ = json_encode($X);
$outJ = set_state($str_inJ);
// Update the custom data and description
$X = db_get_test($test_id);
$X['Updater'] = $X['Creator'];
$V = $X['Variants'];
$newV = array(); $vidx = 0;
foreach ( $V as $v ) { 
  if ( $vidx == 0 ) { 
    $variant_id = $v['id'];
    $newv = $v;
    $newv['custom_data'] = " {} ";
    $newv['description'] = " foo bar ";
  }
  else {
    $newv = $v;
  }
  $newV[$vidx] = $newv;
  $vidx++;
}
$X['Variants'] = $newV;
$X['VariantID'] = $variant_id;
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
?>

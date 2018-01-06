<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'chk_test_basic.php';
require_once 'is_test_name_unique.php';
require_once 'insert_row.php';
require_once 'lkp.php';
require_once 'make_seed.php';

function insert_test(
  $test_name, // name of test, mandatory
  $test_type, // ABTest, XYTest
  $creator,   // name of user who is creating test
  $variant_names, // array: names of variants
  $variant_percs, // array: percentages of variants
  &$test_id 
)
{
  assert(chk_test_basic( $test_name, $test_type, $creator, $variant_names, $variant_percs, )); 
  $abtest_id = -1;
  $d_create =  $d_update = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $GLOBALS["err"] = "";
  $X1['name']         = $test_name;
  $X1['test_type_id'] = lkp("test_type", $test_type);
  $X1['seed']         = make_seed();
  $X1['external_id']  = $t_create;
  $X1['d_create']     = $d_create;
  $X1['t_create']     = $t_create;
  $X1['d_update']     = $d_update;
  $X1['t_update']     = $t_update;
  $X1['creator_id']   = $creator_id;
  $X1['state_id']     = lkp("state", "dormant");
  try {
    $dbh->beginTransaction();
    if ( !is_test_name_unique($test_name) ) {
      throw new Exception("test name [$test_name] not unique");
    }
    insert_row("test", $X1);
    $test_id = $dbh->lastInsertId();
    $X2['test_id']  = $test_id;
    $X2['t_update'] = $t_update;
    $X2['d_update'] = $d_update;
    //-------------------------------------------------------
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X2['percentage']  = $variant_percs[$i];
      $X2['name']        = $variant_names[$i];
      insert_row("variant", $X2);
    }
    //------------------------------------------
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //------------------------------------------
  return true;
}

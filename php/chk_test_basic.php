<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'aux_chk_name.php';

function insert_test(
  $test_name, // name of test, mandatory
  $test_type, // ABTest, XYTest
  $test_dscr, // description, optional
  $creator,   // name of user who is creating test
  $variant_names, // array: names of variants
  $variant_percs, // array: percentages of variants
)
{
  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= $GLOBALS['configs']['max_len_test_dscr'];
  }
  assert(aux_chk_name($test_name));
  assert(lkp("test_type", $test_type));
  assert(lkp("state", "dormant"));


  $d_create =  $d_update = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $GLOBALS["err"] = "";
  $X1['name']         = $name;
  $X1['test_type_id'] = assert(lkp("test_type", $test_type));
  $X1['seed']         = make_seed();
  $X1['external_id']  = $t_create;
  $X1['d_create']     = $d_create;
  $X1['t_create']     = $t_create;
  $X1['d_update']     = $d_update;
  $X1['t_update']     = $t_update;
  $X1['creator_id']   = $creator_id;
  $X1['state_id']     = assert(lkp("state", "dormant"));
  try {
    $dbh->beginTransaction();
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

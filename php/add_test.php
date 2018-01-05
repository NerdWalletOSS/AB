<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "name_to_id.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "aux_check_name.php";
require_once "db_get_variants.php";
require_once "load_configs.php";
require_once "chk_add_test.php";
require_once "make_seed.php";
require_once "get_time_usec.php";

//<hdr>
function add_test(
  $test_name, // name of test, mandatory
  $test_type, // ABTest, XYTest
  $test_dscr, // description, optional
  $creator,   // name of user who is creating test
  $variant_names, // array: names of variants
  $variant_percs, // array: percentages of variants
  $variant_texts, // array: description of variants
  $is_private = 0,
  $channel = "",
  $uses_eee = 1, 
  $is_sequential = 0, 
  &$abtest_id 
)
//</hdr>
{
  $abtest_id = -1;
  $GLOBALS["err"] = "";
  $dbh = dbconn(); 
  if ( $test_type == "XYTest" ) { $uses_eee = 0; }
  $rslt = chk_add_test( $test_name, $test_type, $test_dscr, $creator, 
    $variant_names, $variant_percs, $variant_texts, $channel, 
    $is_sequential);
  if ( !isset($is_private) ) { $is_private = 0; }
  if ( $is_private != 1 ) { $is_private = 0; }
  if ( !$rslt ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }

  $nV = count($variant_names);
  $channel_id   = name_to_id("channel", "name", $channel);
  $test_type_id = name_to_id("test_type", "name", $test_type);
  $creator_id   = name_to_id("admin", "name", $creator);
  $dormant_id   = name_to_id("state", "name", "dormant");
  $d_create =  $d_change = get_date(); 
  $t_create =  $t_change = get_time_usec(); 
  //-----------------------------------
  //----- Prepared statement for inserting test 
  $sql1  = "insert into abtest ";
  $sql1 .= "(name, description, test_type_id, date_created, t_create, external_id, most_recent_update, t_update, ramp_num, num_variants, creator_id, state_id, is_private, is_sequential, channel_id, uses_eee, seed) ";
  $sql1 .= " values (:name, :description, :test_type_id, :d_create, :t_create, :external_id, :d_change, :t_change, :ramp_num, :num_variants, :creator_id, :state_id, :is_private, :is_sequential, :channel_id, :uses_eee, :seed) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['name']         = $test_name;
  $X1['description']  = $test_dscr;
  $X1['test_type_id'] = $test_type_id;
  $X1['d_create']     = $d_create;
  $X1['t_create']     = $t_create;
  if ( $test_type == "ABTest" ) { 
    $X1['external_id']  = 0;
  }
  else {
    $X1['external_id']  = $t_create;
  }
  $X1['d_change']     = $d_change;
  $X1['t_change']     = $t_change;
  $X1['ramp_num']     = 1;
  $X1['num_variants'] = $nV;
  $X1['creator_id']   = $creator_id;
  $X1['state_id']     = $dormant_id; 
  $X1['is_private']   = $is_private;
  $X1['is_sequential']= $is_sequential;
  $X1['channel_id']   = $channel_id;
  $X1['uses_eee']     = $uses_eee;
  $X1['seed']         = make_seed();
  //----- Prepared statement for adding variants
  $sql2  = "insert into variant ";
  $sql2 .= "(name, abtest_id, percentage, description, t_update) ";
  $sql2 .= " values (:name, :abtest_id, :percentage, :description, :t_update) ";
  $stmt2 = $dbh->prepare($sql2);
  $X2['t_update']     = $t_create;
  // prepared statement for ramp history update. Not necessary but 
  // it simplifies things for the DWH team 
  $sql3  = "insert into ramp_history ";
  $sql3 .= "(ramp_num, abtest_id, state_id, modifier_id, ";
  $sql3 .= " variant_id, when_changed, t_change, percentage, count) ";
  $sql3 .= " values (:ramp_num, :abtest_id, :state_id, :modifier_id, ";
  $sql3 .= " :variant_id, :when_changed, :t_change, :percentage, :count) ";
  $stmt3 = $dbh->prepare($sql3);
  $X3['ramp_num']     = 1;
  $X3['state_id']     = $dormant_id;
  $X3['modifier_id']  = $creator_id;
  $X3['when_changed'] = $d_change;
  $X3['t_change']     = $t_change;
  $X3['count']        = 0;
  // Prepared statement for update state history. Not necessary but 
  // it simplifies things for the DWH team 
  $sql4  = "insert into state_history ";
  $sql4 .= "(abtest_id, modifier_id, state_id, ramp_num, ";
  $sql4 .= " when_changed, t_change) "; 
  $sql4 .= " values (:abtest_id, :modifier_id, :state_id, :ramp_num, ";
  $sql4 .= " :when_changed, :t_change) ";
  $stmt4 = $dbh->prepare($sql4);
  $X4['modifier_id']  = $creator_id;
  $X4['state_id']     = $dormant_id;
  $X4['ramp_num']     = 1;
  $X4['when_changed'] = $d_change;
  $X4['t_change']     = $t_change;
  // Start transaction
  //
  try {
    $dbh->beginTransaction();
    // Add the test 
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { 
      #var_dump($stmt1);
      #var_dump($X1);
      throw new Exception("statement 1 failed"); }

    $abtest_id = $dbh->lastInsertId();
    $X2['abtest_id']  = $abtest_id;
    $X4['abtest_id']    = $abtest_id;
    $X3['abtest_id']    = $abtest_id;
    //-------------------------------------------------------
    // Now insert the variants
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X2['percentage']  = $variant_percs[$i];
      $X2['name']        = $variant_names[$i];
      $X2['description'] = $variant_texts[$i];
      $rslt2 = $stmt2->execute($X2); 
      if ( !$rslt2 ) { throw new Exception("statement 2 failed"); }
      $variant_ids[$i] = $dbh->lastInsertId();
    }
    //------------------------------------------
    // update ramp history 
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X3['percentage']   = $variant_percs[$i];
      $X3['variant_id']   = $variant_ids[$i];
      $rslt3 = $stmt3->execute($X3); 
      if ( !$rslt3 ) { throw new Exception("statement 3 failed"); }
    }
    // Update state history
    $rslt4 = $stmt4->execute($X4); 
    if ( !$rslt4 ) { throw new Exception("statement 4 failed"); }
    $dbh->commit();
  } catch ( PDOException $ex ) {
    #var_dump($ex);
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //------------------------------------------
  return true;
}
?>

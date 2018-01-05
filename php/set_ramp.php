<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../tcc/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_set_row.php";
require_once "yurl.php";
require_once "list_rts.php";
require_once "chk_set_ramp.php";

function set_ramp(
  $test_name, 
  $test_type, 
  $variant_spec, // e.g., "Red:10 Blue:20"
  $modifier,
  &$is_change
)
{
  $is_change = true;
  $is_ok = true;
  $GLOBALS["err"] = "";
  if ( !chk_set_ramp( $test_name, $test_type, $variant_spec, $modifier, $is_change, $V) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  if ( !$is_change ) { 
    $GLOBALS["err"] .= "WARN: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  //---------------------------------------------
  $configs = $GLOBALS['CONFIGS'];
  $dbh = dbconn();  
  $modifier_id = name_to_id("admin", "name", $modifier); 
  $test_type_id = name_to_id("test_type", "name", $test_type); 
  $archived_id = name_to_id("state", "name", "archived");
  $T = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id and state_id != $archived_id ");
  if ( !$T ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $curr_ramp_num = $T['ramp_num'];
  $curr_state_id = $T['state_id'];
  $curr_state    = id_to_name($curr_state_id, "state");
  $abtest_id     = $T['pk'];
  $state_id      = $T['state_id'];
  $d_change      = get_date(); 
  $t_change      = get_time_usec(); 
  //------------------------------------------------
  //----- Prepared statement for update abtest 
  $sql1  = "update abtest set ramp_num = :ramp_num, ";
  $sql1 .= "t_update = :t_update, most_recent_update = :d_update ";
  $sql1 .= " where pk = :abtest_id";
  $stmt1 = $dbh->prepare($sql1);
  $X1['ramp_num']    = $curr_ramp_num + 1;
  $X1['t_update']    = $t_change;
  $X1['d_update']    = $d_change;
  $X1['abtest_id']   = $abtest_id;
  //----- Prepared statement for updating variants
  $sql2  = "update variant set count = 0, t_update = :t_update,  ";
  $sql2 .= " percentage = :percentage where pk = :pk ";
  $X2['t_update'] = $t_change;
  $stmt2 = $dbh->prepare($sql2);
  // prepared statement for ramp history insert. 
  $sql3  = "insert into ramp_history ";
  $sql3 .= "(ramp_num, abtest_id, state_id, modifier_id, ";
  $sql3 .= " variant_id, when_changed, t_change, percentage, count) ";
  $sql3 .= " values (:ramp_num, :abtest_id, :state_id, :modifier_id, ";
  $sql3 .= " :variant_id, :when_changed, :t_change, :percentage, :count) ";
  $stmt3 = $dbh->prepare($sql3);
  $X3['ramp_num']     = $curr_ramp_num + 1;
  $X3['abtest_id']    = $abtest_id;
  $X3['state_id']     = $curr_state_id;
  $X3['modifier_id']  = $modifier_id;
  $X3['when_changed'] = $d_change;
  $X3['t_change']     = $t_change;
  $X3['count']        = 0;
  // prepared statement for ramp history update. 
  $sql4  = "update ramp_history set modifier_id = :modifier_id, ";
  $sql4 .= "percentage = :percentage, when_changed = :when_changed, ";
  $sql4 .= "t_change = :t_change where abtest_id = :abtest_id and ";
  $sql4 .= "variant_id = :variant_id ";
  $stmt4 = $dbh->prepare($sql4);
  $X4['abtest_id']    = $abtest_id;
  $X4['modifier_id']  = $modifier_id;
  $X4['when_changed'] = $d_change;
  $X4['t_change']     = $t_change;
  //----------------------------------------------------
  if ( $curr_state == "dormant" ) {
    // update proportions in variant and ramp history and early exit 
    try {
      $dbh->beginTransaction();
      // Update the test just for modifier and time 
      $X1['ramp_num']    = 1;
      $rslt1 = $stmt1->execute($X1); 
      if ( !$rslt1 ) { throw new Exception("statement 1 failed"); }
      // Now update the variants
      foreach ( $V as $v ) { 
        $X2['pk'] = $v['pk'];
        $X2['percentage'] = $v['percentage'];
        $rslt2 = $stmt2->execute($X2); 
        if ( !$rslt2 ) { throw new Exception("statement 2 failed"); }
        // update ramp history
        $X4['percentage']  = $v['percentage'];
        $X4['variant_id']  = $v['pk'];
        $rslt4 = $stmt4->execute($X4); 
        if ( !$rslt4 ) { throw new Exception("statement 4 failed"); }
      }
      $dbh->commit();
    } catch ( PDOException $ex ) {
      $dbh->rollBack();
      var_dump($ex);
      $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    return true;
  }
  //------------------------------------------------------
  try {
    $dbh->beginTransaction();
    // Update the test 
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { throw new Exception("statement 1 failed"); }
    //-------------------------------------------------------
    // Now update the variants
    foreach ( $V as $v ) { 
      $X2['pk'] = $v['pk'];
      $X2['percentage'] = $v['percentage'];
      $rslt2 = $stmt2->execute($X2); 
      if ( !$rslt2 ) { throw new Exception("statement 2 failed"); }
    }
    // add new line to ramp history 
    foreach ( $V as $v ) { 
      $X3['variant_id'] = $v['pk'];
      $X3['percentage'] = $v['percentage'];
      $rslt3 = $stmt3->execute($X3); 
      if ( !$rslt3 ) { throw new Exception("statement 3 failed"); }
    }
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    var_dump($ex);
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  // Figure out whether an RTS is available 
  $SP = list_rts();
  if ( !is_array($SP) ) { 
    $GLOBALS["err"] .= "WARN: Not talking to RTS\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  //--- Create URL for RTS 
  $url = "SetPercentages?TestName=$test_name&TestType=$test_type";
  foreach ( $V as $v ) { 
    $name = $v['name'];
    $percentage = $v['percentage'];
    $url .= "&$name=$percentage";
  }
  //---- Inform RTS 
  if ( isset($SP) ) { 
    foreach ( $SP as $sp ) { 
      $server = $sp['server']; $port = $sp['port']; 
      $rslt = ""; $http_code = 0;
      yurl($server, $port, $url, $http_code, $rslt);
      if ( $http_code != 200 ) { 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $GLOBALS["err"] .= "HTTP Code is $http_code\n. RTS says $rslt";
        $is_ok = false;
      }
    }
  }
  return $is_ok;
}
?>

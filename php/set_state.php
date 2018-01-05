<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_variant.php";
require_once "get_time_usec.php";
require_once "yurl.php";
require_once "list_rts.php";
require_once "chk_set_state.php";
require_once "mk_add_test_url.php";
require_once "load_configs.php";

function set_state(
  $test_name,
  $test_type, 
  $new_state,
  $modifier,
  &$is_change,
  $final_variant
)
{
  $GLOBALS['err'] = "";
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  // Following values are set inside chk_set_state
  $abtest_id        = -1;
  $ramp_num         = -1;
  $final_variant_id = -1;
  $new_state_id     = -1;
  $modifier_id      = -1;
  $is_change        = false;

  if ( !chk_set_state( $test_name, $test_type, $new_state, $modifier,
    $final_variant, $abtest_id, $ramp_num, $final_variant_id, 
    $new_state_id, $modifier_id, $is_change) ) { 
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
  if ( !$is_change ) { 
    $GLOBALS["err"] .= "No change in state.";
    return true;
  }
  $configs = $GLOBALS['CONFIGS'];
  $dbh = dbconn();  
  // START: Error checking on inputs 
  //---------------------------------------------
  $t_change = get_time_usec();
  $d_change = get_date();
  // --- Prepare query for Insert into state history
  $sql1  = "insert into state_history ";
  $sql1 .= "(abtest_id, modifier_id, state_id, ramp_num, when_changed, t_change) values ";
  $sql1 .= "(:abtest_id, :modifier_id, :state_id, :ramp_num, :when_changed, :t_change) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['abtest_id']    = $abtest_id;
  $X1['modifier_id']  = $modifier_id;
  $X1['when_changed'] = $d_change;
  $X1['t_change']     = $t_change;
  $X1['state_id']     = $new_state_id;
  $X1['ramp_num']     = $ramp_num;
  //--- Prepare query for update of abtest
  $sql2  = "update abtest set state_id = :new_state_id, ";
  $sql2 .= "most_recent_update = :most_recent_update, ";
  $sql2 .= "t_update = :t_update ";
  $sql2 .= "where pk = :abtest_id ";
  $stmt2 = $dbh->prepare($sql2);
  $X2['most_recent_update'] = $d_change;
  $X2['t_update']           = $t_change;
  $X2['new_state_id']       = $new_state_id;
  $X2['abtest_id']          = $abtest_id;
  // Prepare query for setting of is_final in abtest 
  $sql3  = "update variant set is_final = 1, t_update = :t_update where pk = :final_variant_id";
  $stmt3 = $dbh->prepare($sql3);
  $X3['final_variant_id'] = $final_variant_id;
  $X3['t_update']         = $t_change;

  try {
    $dbh->beginTransaction();
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { throw new Exception("statement 1 failed"); }
    $rslt2 = $stmt2->execute($X2); 
    if ( !$rslt2 ) { throw new Exception("statement 2 failed"); }
    if ( $new_state == "terminated" ) {
      $rslt3 = $stmt3->execute($X3); 
      if ( !$rslt3 ) { throw new Exception("statement 3 failed"); }
    }
    $dbh->commit();
  } catch(PDOException $ex) {
    $dbh->rollBack();
    var_dump($ex);
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //----------------------------------------------------------
  $SP = list_rts();
  if ( !is_array($SP) ) { // NOTHING MORE TO DO 
    $GLOBALS["err"] .= "NOTICE: Not talking to RTS\n";
    return true;  
  } 
  //----------------------------------------------------------
  unset($url);
  switch ( $new_state ) {
  case "started" : 
    $url = mk_add_test_url($test_name, $test_type);
    break;
  case "terminated" :
    $url = "StopTest?TestName=$test_name&TestType=$test_type&FinalVariant=$final_variant";
    break;
  case "archived" :
    $url = "DelTest?TestName=$test_name&TestType=$test_type";
    break;
  default :
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
    break;
  }
  if ( !$url ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //--------------------------------------------------------------
  $is_ok = true;
  foreach ( $SP  as $sp ) {
    $rslt = ""; $http_code = 0;
    $server = $sp['server']; $port   = $sp['port'];
    yurl($server, $port, $url, $http_code, $rslt);
    if ( $http_code != 200 ) { 
      $GLOBALS["err"] .= "Code = $http_code, Error [$rslt] from RTS on $url\n"; 
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $is_ok = false;
    }
    //----------------------------------------------------------
    if ( ( $new_state == "started" ) && ( $test_type == "ABTest" ) ) {
      $sql4  = " select c.name as rskey, ";
      $sql4 .= " b.name as rsval, ";
      $sql4 .= " a.abtest_id as abtest_id ";
      $sql4 .= " from cat_attr_val_test a ";
      $sql4 .= " JOIN cat_attr c ON a.cat_attr_id     = c.pk ";
      $sql4 .= " JOIN cat_attr_val b ON a.cat_attr_val_id = b.pk ";
      $sql4 .= " where a.abtest_id = $abtest_id ";
      $stmt4 = $dbh->prepare($sql4);
      $rslt4 = $stmt4->execute(); 
      if ( $rslt4 ) { 
        for ( ; ; ) { 
          $row = $stmt4->fetch(PDO::FETCH_ASSOC);
          if ( !$row ) { break; }
          $attr_name = $row['rskey'];
          $attr_val  = $row['rsval'];
          $url = "SetCatAttrValTest?TestName=$test_name&Attribute=$attr_name&Value=$attr_val";
          $http_code = 0; $rslt = "";
          $server = $sp['server'];
          $port   = $sp['port'];
          yurl($server, $port, $url, $http_code, $rslt);
          if ( $http_code != 200 ) {
            $GLOBALS["err"] .= "ERR: RTS ($server) says $rslt \n";
            $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            $is_ok = false;
          }
        }
      }
    }
    //----------------------------------------------------------
    if ( ( $new_state == "started" ) && ( $test_type == "XYTest" ) ) { 
      $V = db_get_variants($abtest_id);
      if ( !$V ) { 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
      foreach ( $V as $v ) {
        $set_url = "";
        $set_url .= "SetURL?";
        $set_url .= "TestName="     . $test_name;
        $set_url .= "&VariantName=" . $v["name"]; 
        $set_url .= "&VariantURL="  . $v["description"]; 
        // IMPORTANT: VariantURL must be the LAST entry in the URL
        // This is an unfortunate but (for now) necessary hack
        //-------------------------------------
        $rslt = ""; $http_code = 0;
        yurl($server, $port, $set_url, $http_code, $rslt);
        if ( $http_code != 200 ) { 
          $GLOBALS["err"] .= "Error [$rslt] from RTS on $set_url\n"; 
          $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          $is_ok = false;
        }
      }
      // Create URL for device specific configs 
      // TestName=T1&ExternalID=123&
      // DeviceSpecific=[1,3,50,1,2,50,2,3,80,2,2,20]
      $T = db_get_row("abtest", "pk", $abtest_id);
      if ( !$T ) { 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      $is_dev_specific = $T['is_dev_specific'];
      $ramp_num           = $T['ramp_num'];
      if ( $is_dev_specific ) { 
        $sql0  = "select * from device_x_variant where 
          percentage > 0 and ramp_num = $ramp_num and 
          variant_id in (select pk from variant where abtest_id = $abtest_id )";
        $stmt0 = $dbh->prepare($sql0);
        $rslt0 = $stmt0->execute(); 
        if ( !$rslt0 ) { 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
        }
        $str = "";
        for ( ; ; ) { 
          $row0 = $stmt0->fetch(PDO::FETCH_ASSOC);
          if ( !$row0 ) { break; }
          $device_id  = $row0['device_id'];
          $percentage = $row0['percentage'];
          $variant_id = $row0['variant_id'];
          $str .= "$device_id,$percentage,$variant_id;";
        }
        $set_url = "";
        $set_url .= "SetDevSpecPerc?";
        $set_url .= "TestName="     . $test_name;
        $set_url .= "&DeviceSpecificPerc="     . $str;
        $rslt = ""; $http_code = 0;
        yurl($server, $port, $set_url, $http_code, $rslt);
        if ( $http_code != 200 ) { 
          $GLOBALS["err"] .= "Error [$rslt] from RTS on $set_url\n"; 
          $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          $is_ok = false;
        }
      }
    }
  }
  return $is_ok;
}
?>

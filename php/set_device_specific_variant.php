<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "dbconn.php";
require_once "aux_check_name.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "db_get_variants.php";
require_once "load_configs.php";
require_once "db_logger.php";

function 
get_variant_id(
    $V, 
    $url
)
{
  foreach ( $V as $v ) { 
    if ( $url == $v['description'] ) { 
      return($v['pk']);
    }
  }
  var_dump($V);
  var_dump($url);
  return null;
}

function
set_device_specific_variant(
  $test_name,
  $J
)
{
  $status = false;
  $dbh = dbconn();  
  $GLOBALS['err'] = "";
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $configs = $GLOBALS['CONFIGS'];
  $max_num_devices      = $configs['max_num_devices'];

  $xy_test_id  = name_to_id("test_type", "name", "XYTest");
  $dormant_id  = name_to_id("state",     "name", "dormant");
  $started_id  = name_to_id("state",     "name", "started");
  $terminated_id = name_to_id("state",   "name", "terminated");
  $test_name = trim($test_name);
  $where_clause  = "and test_type_id = $xy_test_id and "; 
  $where_clause .= "( ( state_id = $started_id ) or ( state_id = $dormant_id ) ) ";

  $T = db_get_row("abtest", "name", $test_name,  $where_clause);
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid Test [$test_name]\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $abtest_id    = $T['pk'];
  $state_id     = $T['state_id'];
  if ( $state_id == $dormant_id ) {
    $ramp_num     = $T['ramp_num'];
    if ( $state_id == $dormant_id ) { 
      $sql  = "update abtest set is_dev_specific = 1 where pk = $abtest_id";
      $stmt = $dbh->prepare($sql);
      $rslt = $stmt->execute();
      if ( !$rslt ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $status = false; goto BYE;
      }
    }
    else {
    }
  }
  else {
    if ( !$T['is_dev_specific'] ) { 
      $GLOBALS["err"] .= "Must set device specific when dormant\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
    $ramp_num     = $T['ramp_num'] + 1;
  }

  $V = db_get_variants($abtest_id);
  if ( !$V) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }

  // $tempt is array in which we will temporarily store stuff
  $ttidx = 0; // index for tempt
  $X = json_decode($J);
  if ( !$X) { 
    $GLOBALS["err"] .= "Invalid JSON input \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $num_devices = db_num_rows("device");
  if ( ( $num_devices == 0 ) || ( $num_devices > $max_num_devices ) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }

  $devidx = 0; // index for devices
  foreach ( $X as $device => $url_perc ) {
    $R = db_get_row("device", "name", $device);
    if ( !$R ) { 
      $GLOBALS["err"] .= "No such device [$device]\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
    $devices[$devidx++] = $device;
    $device_id = $R['pk'];
    $sum = 0;
    unset($variant_ids);
    unset($urls);
    $idx = 0;
    foreach ( $url_perc as $url => $perc ) { 
      // Do not specify URLs that have no percentage
      if ( ( $perc <= 0 ) || ( $perc > 100 ) ) { 
        $GLOBALS["err"] .= "Invalid percentage [$perc]\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $status = false; goto BYE;
      }
      $sum += $perc;
      $variant_id = get_variant_id($V, $url);
      if ( !$variant_id ) { 
        $GLOBALS["err"] .= "Invalid variant [$url]\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $status = false; goto BYE;
      }
      $tempt[$ttidx++] = array(
        "device_id" => $device_id, 
        "percentage" => $perc, 
        "variant_id" => $variant_id);
      $variant_ids[$idx] = $variant_id;
      $urls[$idx]        = $url;
      $idx++;
    }
    for ( $i = 0; $i < $idx; $i++ ) { 
      for ( $j = $i+1; $j < $idx; $j++ ) { 
        if ( $variant_ids[$i] == $variant_ids[$j] ) {
          $GLOBALS["err"] .= "Duplicate URLS for same device [$device]\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          $status = false; goto BYE;
        }
        if ( $urls[$i] == $urls[$j] ) {
          $GLOBALS["err"] .= "Duplicate URLS for same device [$device]\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          $status = false; goto BYE;
        }
      }
    }
    if ( $sum != 100 ) { 
      $GLOBALS["err"] .= "percentages do not add up to 100 [$sum]\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
  }
  if ( count($devices) != $num_devices ) { 
    $GLOBALS["err"] .= "Not all devices specified\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  if ( $ttidx == 0 ) {
    $GLOBALS["err"] .= "Nothing to do \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  // If dormant, delete older stuff for this test 
  if ( $state_id == $dormant_id ) { 
    $sql0  = "delete from device_x_variant where variant_id in (select pk from variant where abtest_id = $abtest_id )";
    $stmt0 = $dbh->prepare($sql0);
    $rslt0 = $stmt0->execute();
    if ( !$rslt0 )  {
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      $status = false; goto BYE;
    }
  }
  // Now insert tempt into device_x_variant
  $url = "SetDevSpecPerc?TestName=$test_name&DeviceSpecificPerc=";
  $n_before = db_num_rows("device_x_variant");
  $sql1  = "insert into device_x_variant ";
  $sql1 .= "(ramp_num, device_id, variant_id, percentage) values ";
  $sql1 .= "(:ramp_num, :device_id, :variant_id, :percentage) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['ramp_num']         = $ramp_num;
  try {
    $dbh->beginTransaction();
    foreach ( $tempt as $tempr ) { 
      $X1['device_id']  = $tempr['device_id'];
      $X1['percentage'] = $tempr['percentage'];
      $X1['variant_id'] = $tempr['variant_id'];
      $str = $X1['device_id'] . "," .$X1['percentage'] . "," . $X1['variant_id'] . ";";
      $url .= $str;
      $rslt1 = $stmt1->execute($X1); 
      if ( !$rslt1 ) { 
        var_dump($stmt1);
        var_dump($X1);
        throw new Exception("statement 1 failed"); }
    }
    $dbh->commit();
  } catch ( PDOException $ex ) {
    var_dump($ex);
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $n_after  = db_num_rows("device_x_variant");
  if ( $n_after - count($tempt)  != $n_before ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  // update ramp num in abtest 
  if ( $state_id == $started_id ) {
    $sql  = "update abtest set ramp_num = $ramp_num where pk = $abtest_id";
    $stmt = $dbh->prepare($sql);
    $rslt = $stmt->execute();
  }
  //----------------------------------------------------
  if ( $state_id != $dormant_id ) { // Inform RTS
    $SP = list_rts();
    if ( !is_array($SP) ) { // NOTHING MORE TO DO 
      $GLOBALS["err"] .= "NOTICE: Not talking to RTS\n";
      return true;  
    } 
    foreach ( $SP  as $sp ) {
      $rslt = ""; $http_code = 0;
      $server = $sp['server']; $port   = $sp['port'];
      yurl($server, $port, $url, $http_code, $rslt);
      if ( $http_code != 200 ) { 
        $GLOBALS["err"] .= "Error [$rslt] from RTS on $url\n"; 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        $status = false; goto BYE;
      }
    }
  }
BYE:
  db_logger($J, $status, "SetDeviceSpecific");
  return $status;
}

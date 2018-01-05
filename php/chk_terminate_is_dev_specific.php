<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "load_configs.php";

function
chk_terminate_is_dev_specific(
  $test_name
)
{
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $configs = $GLOBALS['CONFIGS'];
  $max_num_devices      = $configs['max_num_devices'];

  $xy_test_id  = name_to_id("test_type", "name", "XYTest");
  $started_id  = name_to_id("state",     "name", "started");
  $test_name = trim($test_name);
  $where_clause  = "and test_type_id = $xy_test_id and "; 
  $where_clause .= " state_id = $started_id ";

  $T = db_get_row("abtest", "name", $test_name,  $where_clause);
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid Test [$test_name]\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $abtest_id    = $T['pk'];
  $state_id     = $T['state_id'];
  $ramp_num     = $T['ramp_num'];

  $V = db_get_variants($abtest_id);
  if ( !$V) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }

  $V = db_get_variants($abtest_id);
  $vids = ""; $first = true;
  foreach ( $V as $v ) {
    if ( $first ) { $first = false; } else { $vids .= ", "; }
    $vids .= $v['pk'];
  }
  $dbh = dbconn();
  $sql  = "select * from device_x_variant where variant_id in ($vids) ";
  $sql .= " and ramp_num = :ramp_num ";
  $sql .= " and device_id = :device_id ";
  $stmt = $dbh->prepare($sql);
  $X['ramp_num']  = $ramp_num;

  $D = db_get_rows("device");
  foreach ($D as $d ) { 
    $X['device_id']  = $d['pk'];
    $rslt = $stmt->execute($X); 
    if ( !$rslt ) { 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
    $hundred_seen = false;
    $num_rows = 0;
    for ( ; ; ) {
      $row = $stmt->fetch(PDO::FETCH_ASSOC);
      if ( !$row ) { break; }
      $num_rows++;
      $percentage = $row['percentage'];
      if ( $hundred_seen ) {
        if ( $percentage != 0 ) { 
          $GLOBALS["err"] .= "Should be only 100 in each column\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
      }
      else {
        if ( $percentage == 100 ) { 
          $hundred_seen = true;
        }
        else if ( $percentage != 0 ) {
          $GLOBALS["err"] .= "Should be only 100 in each column\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false;
        }
      }
    }
    if ( $num_rows == 0 ) { 
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
  }
  return true;
}

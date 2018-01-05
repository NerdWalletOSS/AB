<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "db_set_row.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_get_rows.php";
require_once "name_to_id.php";
require_once "db_num_rows.php";
require_once "db_get_variants.php";
require_once "load_configs.php";

function get_device_specific(
  $test_name, // name of test, mandatory
  $test_id=0 // used for archived tests where can have same name
)
//</hdr>
{
  $GLOBALS["err"] = "";
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true; 
  }
  $configs = $GLOBALS['CONFIGS'];
  //---------------------------------------
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "Cannot connect to database\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $test_name = trim($test_name); 
  $xy_test_id = name_to_id("test_type", "name", "XYTest");
  $archived_id = name_to_id("state", "name", "archived");
  if ( $test_name == "" ) { 
    $T = db_get_row("abtest", "pk", $test_id);
  }
  else {
    $T = db_get_row("abtest", "name", $test_name, 
      " and state_id != $archived_id and test_type_id = $xy_test_id ");
  }
  if ( !$T ) { 
    $GLOBALS["err"] .= "Test name not valid \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $ramp_num  = $T['ramp_num'];
  $abtest_id = $T['pk'];

  unset($X);
  $X['ramp_num'] = $ramp_num;
  $sql  = "select percentage from device_x_variant where ";
  $sql .= " ramp_num = :ramp_num and ";
  $sql .= " variant_id = :variant_id and  ";
  $sql .= " device_id = :device_id ";
  $stmt = $dbh->prepare($sql);
  /*
  for ( ; ; ) { 
    $row = $stmt->fetch(PDO::FETCH_ASSOC);
    if ( !$row ) { break; }
    $device_id  = $row['device_id']; 
    $variant_id = $row['variant_id']; 
    $percentage = $row['percentage']; 
    $counts[$row['device_id'][$row['variant_id']]] = $row['percentage'];
  }
   */
  //---------------------------------
  $D = db_get_rows("device");
  $iD = array(count($D));
  $didx = 0;
  foreach ( $D as $d ) { 
    $iD[$didx++] = $d['name'];
  }
  //------------------------------------------
  $U = db_get_rows("variant", " abtest_id = $abtest_id ");
  $iU = array(count($U));
  $uidx = 0;
  foreach ( $U as $u ) {
    $iU[$uidx++] = $u['description'];
  }
  //------------------------------------------
  $uidx = 0;
  foreach ( $U as $u ) {
    $key = "prop_$uidx";
    $variant_id = $u['pk'];
    $X['variant_id'] = $variant_id;
    $didx = 0;
    foreach ( $D as $d ) {
      $key = "prop_" . $uidx . "_" . $didx;
      $device_id = $d['pk'];
      $X['device_id'] = $device_id;
      $rslt = $stmt->execute($X); 
      if ( !$rslt ) { 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
      $row = $stmt->fetch(PDO::FETCH_ASSOC);
      if ( !$row ) { 
        $percentage = 0;
      }
      else {
        $percentage = $row['percentage'];
      }
      $P[$key] = $percentage;
      $didx++;
    }
    $uidx++;
  }

  $retval['devices']     = $iD;
  $retval['urls']        = $iU;
  $retval['percentages'] = $P;

  return $retval;
}
/*
$x = get_device_specific("", 4);
var_dump($x);
var_dump($GLOBALS['err']);
 */

?>

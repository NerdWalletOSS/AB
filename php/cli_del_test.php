<?php
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_get_rows.php";
require_once "name_to_id.php";

function del_abtest_fk(
  $tbl,
  $key,
  $val
)
{
  $sql = "delete from $tbl where $key = $val";
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); 
  if ( !$rslt ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  return true;
}

function del_test(
  $test_id
)
{
  //--------------------------------------------------------
  $rslt = del_abtest_fk("variant", "test_id", $test_id);
  if ( !$rslt ) { return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("cat_attr_val_test", "test_id", $test_id);
  if ( !$rslt ) { print("ERROR: $test_id \n"); return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("abtest", "pk", $test_id);
  if ( !$rslt ) { print("ERROR: $test_id \n"); return false; }
  //--------------------------------------------------------
  return true;
}
if ( $argc != 2 ) { echo "Expected 3 arguments. Got $argc"; exit(1); }
$test_id = $argv[1];
rs_assert(is_numeric($test_id));
del_test($test_id);

/*
$R = db_get_rows("abtest", " (state_id = 1) ");
if ( !is_null($R) ) {
  foreach ( $R as $r ) {
    $pk = $r['pk'];
    $test_name = $r['name'];
    $test_type_id = $r['test_type_id'];
    $state_id = $r['state_id'];
    $test_type = id_to_name($test_type_id,  "test_type");
    $x = del_test($test_name, $test_type);
    if ( !$x ) { 
      var_dump($GLOBALS['err']); 
      exit(1); 
    }
    echo "Deleted $test_name\n";
  }
}
else {
  print("Nothing to delete \n");
}

print("Successfully completed \n");
 */
?>

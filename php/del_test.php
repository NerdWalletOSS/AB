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
  $name,
  $type
)
{
  $GLOBALS["err"] = "";
  $rslt = db_get_row("test_type", "name", $type);
  if ( !$rslt ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  $test_type_id = $rslt['pk'];

  $dormant_id   = name_to_id("state", "name", "dormant");

  $row = db_get_row("abtest", "name", $name, 
    " and test_type_id = $test_type_id and state_id = $dormant_id ");
  if ( !$row ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . ": LINE: " . __LINE__;
    return false; 
  }
  $abtest_id = $row["pk"];
  //--------------------------------------------------------
  $rslt = del_abtest_fk("state_history", "abtest_id", $abtest_id);
  if ( !$rslt ) { return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("ramp_history", "abtest_id", $abtest_id);
  if ( !$rslt ) { return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("variant", "abtest_id", $abtest_id);
  if ( !$rslt ) { return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("cat_attr_val_test", "abtest_id", $abtest_id);
  if ( !$rslt ) { print("ERROR: $abtest_id \n"); return false; }
  //--------------------------------------------------------
  // Gather all abtest_aux_id's 
  $R = db_get_rows("abtestaux", "abtest_id = $abtest_id ");
  if ( !is_null($R) ) {
    foreach ( $R as $r ) {
      $abtest_aux_id = $r['pk'];
      $rslt = del_abtest_fk("testurls", "abtest_aux_id", $abtest_aux_id);
      if ( !$rslt ) { return false; }
      $rslt = del_abtest_fk("primarymetrics", "abtest_aux_id", $abtest_aux_id);
      if ( !$rslt ) { return false; }
    }
  }
  $rslt = del_abtest_fk("abtestaux", "abtest_id", $abtest_id);
  if ( !$rslt ) { print("ERROR: $abtest_id \n"); return false; }
  //--------------------------------------------------------
  $rslt = del_abtest_fk("abtest", "pk", $abtest_id);
  if ( !$rslt ) { print("ERROR: $abtest_id \n"); return false; }
  //--------------------------------------------------------
  return true;
}
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

<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'dbconn.php';
require_once 'rs_assert.php';
function inv2(
)
{
  $dbh = dbconn();
  rs_assert($dbh);
  $sql  = "SELECT test_id, device_id, sum(percentage) as percentage ";
  $sql .= " FROM device_x_variant GROUP BY test_id, device_id";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return true; }

  for ( $i =  0; $i < $nR; $i++ ) { 
    $row = $stmt->fetch(PDO::FETCH_ASSOC);
    $percentage  = $row['percentage']; 
    $test_id     = $row['test_id'];
    $device_id   = $row['device_id'];
    rs_assert($percentage == 100, 
      "Error: test $test_id, device_id $device_id, perc = $percentage\n");
  }
  return true;
}
inv2();
?>

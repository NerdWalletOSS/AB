<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";

function db_get_tests(
  $test_type,
  $is_archived=false
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $rslt = db_get_row("state", "name", "archived");
  $archived_id = $rslt['pk'];
  //-----------------------------------
  if ( ( is_null($test_type) ) || ( $test_type == "" ) ) {
    $sql  = "select * from abtest where ";
  }
  else {
    $rslt = db_get_row("test_type", "name", $test_type);
    $test_type_id = $rslt['pk']; 
    $sql  = "select * from abtest where test_type_id = $test_type_id ";
  }
  //-----------------------------------
  if ( $is_archived ) { 
    $sql  .= " and state_id =  $archived_id ";
  }
  else {
    $sql  .= " and state_id != $archived_id ";
  }
  $sql  .= " order by pk ";
  //-------------------------------------------------------------
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  $X = array($nR);
  for ( $i = 0; $i < $nR; $i++ ) { 
    $X[$i] = $stmt->fetch(PDO::FETCH_ASSOC);
  }
  return $X;
}
?>

<?php
require_once "dbconn.php";
require_once "db_num_rows.php";
require_once "name_to_id.php";

function get_scn(
)
{
  $dbh = dbconn();
  if ( !$dbh ) { return null; }
  $dormant_id   = name_to_id("state", "name", "dormant");
  $archived_id   = name_to_id("state", "name", "archived");
  $nR = db_num_rows("abtest", 
    " ( state_id != $dormant_id ) and ( state_id != $archived_id ) ");
  if ( $nR == 0 ) { return 0; }
  $w1 = " where ( state_id != $dormant_id ) and ( state_id != $archived_id ) "; 

  //----------------------------------------
  $sql  = "select max(t_update) from abtest $w1";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $row = $stmt->fetch();
  $t0 = $row[0];
  $w2 = " where abtest_id in (select pk from abtest $w1 ) ";
  //----------------------------------------
  $sql  = "select max(t_update) from variant $w2";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $row = $stmt->fetch();
  $t1 = $row[0];
  //----------------------------------------
  $sql  = "select max(t_change) from state_history $w2";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $row = $stmt->fetch();
  $t2 = $row[0];
  //----------------------------------------
  $sql  = "select max(t_change) from ramp_history $w2";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $row = $stmt->fetch();
  $t3 = $row[0];
  //----------------------------------------
  $t_state = $t0;
  if ( $t1 > $t_state ) { $t_state = $t1; }
  if ( $t2 > $t_state ) { $t_state = $t2; }
  if ( $t3 > $t_state ) { $t_state = $t3; }
  return $t_state;
}
// echo get_scn();
?>

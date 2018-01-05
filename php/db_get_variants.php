<?php
require_once "bye.php";
require_once "dbconn.php";

function db_get_variants(
  $abtest_id
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return null;
  }
  //-----------------------------------
  $sql  = "select * from variant where abtest_id = $abtest_id order by pk asc";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { 
    $GLOBALS["err"] .= "bad variants for abtest_id = $abtest_id \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return null; 
  }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return null; 
  }
  $X = array($nR);
  for ( $i = 0; $i < $nR; $i++ ) {
    $X[$i] = $stmt->fetch(PDO::FETCH_ASSOC);
  }
  return $X;
}
// $X = db_get_variants(1);
// var_dump($X);
?>

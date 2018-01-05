<?php
require_once "bye.php";
require_once "dbconn.php";

function load_lkp_tbl(
  $tbl
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $sql  = "select * from $tbl";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  $X = array($nR);
  for ( $i = 0; $i < $nR; $i++ ) {
    $row   = $stmt->fetch(PDO::FETCH_ASSOC);
    $pk = $row['pk'];
    $X[$pk] = $row; 
  }
  return $X;
}
if ( getenv("ENABLE_PHP_CLI") != "" ) { 
  $X = load_lkp_tbl("state");
  var_dump($X);
}
?>

<?php
require_once 'dbconn.php';
function db_get_rows(
  $tbl,
  $where_clause=""
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "select * from $tbl ";
  if ( ( !is_null($where_clause) ) && ( $where_clause != "" ) )  {
    $sql .= " where $where_clause ";
  }
  $stmt = $dbh->prepare($sql);
  // print("db_get_rows: sql = $sql \n");
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  $rslt = array($nR);
  for ( $i =  0; $i < $nR; $i++ ) { 
    $rslt[$i] = $stmt->fetch(PDO::FETCH_ASSOC);
  }
  return $rslt;
}
?>

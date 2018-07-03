<?php
function db_num_rows(
  $tbl,
  $where_clause = ""
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }
  $where_clause = trim($where_clause);

  $sql  = "select * from $tbl ";
  if ( $where_clause != "" ) { $sql .= " where $where_clause "; }
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute(); if ( !$rslt ) { go_BYE(""); }
  if ( !$rslt ) { return 0; }
  $nR = $stmt->rowCount();
  return $nR;
}
?>

<?php
function db_set_row(
  $tbl,
  $fld,
  $value,
  $where_clause
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "update $tbl set $fld = :value $where_clause";
  $stmt = $dbh->prepare($sql);
  $X['value'] = $value;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE(""); }
  return true;
}
?>

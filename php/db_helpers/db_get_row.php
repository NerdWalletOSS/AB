<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once 'bye.php';
require_once 'dbconn.php';
function db_get_row(
  $tbl,
  $key,
  $value,
  $additional_where_clause = ""
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "select * from $tbl where $key = :value ";
  $sql .= " $additional_where_clause ";
  $stmt = $dbh->prepare($sql);
  $X['value'] = $value;
  // echo "$sql\n"; var_dump($X);
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE(""); }
  $nR = $stmt->rowCount();
  if ( $nR != 1 ) { return null; }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row;
}
?>

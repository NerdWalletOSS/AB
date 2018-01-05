<?php
function name_to_id(
  $tbl,
  $key,
  $value
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "select pk from $tbl where $key = :value ";
  $stmt = $dbh->prepare($sql);
  $X['value'] = $value;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE(""); }
  $nR = $stmt->rowCount();
  if ( $nR != 1 ) { return null; }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row['pk'];
}
function id_to_name(
  $value,
  $tbl
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "select name from $tbl where pk = :value ";
  $stmt = $dbh->prepare($sql);
  $X['value'] = $value;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE(""); }
  $nR = $stmt->rowCount();
  if ( $nR != 1 ) { return null; }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row['name'];
}
?>

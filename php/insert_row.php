<?php
function insert_row(
  $tbl,
  $kvs
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "insert into $tbl ( ";
  $first = true;
  $save_kvs = $kvs;
  foreach ( $kvs as $key => $val ) {
    if ( is_null($val) ) { continue; }
    if ( $first ) { 
      $first = false;
    }
    else {
      $sql .= ", ";
    }
    $sql .= " $key ";
    $X["$key"] = $val;
  }
  $sql  .= " ) values ( ";
  $first = true;
  foreach ( $kvs as $key => $val ) {
    if ( is_null($val) ) { continue; }
    if ( $first ) { 
      $first = false;
    }
    else {
      $sql .= ", ";
    }
    $sql .= ":$key ";
  }
  $sql  .= " ) ";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute($X); 
  if ( !$rslt ) { 
    var_dump($sql);
    var_dump($X);
  }
  rs_assert($rslt);
  $pk = $dbh->lastInsertId();
  return $pk;
  /*
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  $rslt = array($nR);
  for ( $i =  0; $i < $nR; $i++ ) { 
    $rslt[$i] = $stmt->fetch(PDO::FETCH_ASSOC);
  }
   */
  return $pk;
}
?>

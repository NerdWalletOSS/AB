<?php
require_once 'dbconn.php';
function db_set_row(
  $tbl,
  $id,
  $kvs
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $first = true;
  $sql  = "update $tbl set ";
  foreach ( $kvs as $key => $val ) {
    if ( is_null($val) ) { continue; }
    if ( $first ) { 
      $first = false;
    }
    else {
      $sql .= ", ";
    }
    $sql .= " $key = :$key";
    $X["$key"] = $val;
  }
  $sql .= " where id = $id ";

  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute($kvs); if ( !$rslt ) { return null; }
  // var_dump($sql); var_dump($kvs);
  // TODO: Check number of affected rows is 1 */
  return true;
}
/* 
$X['name'] = "abcdef";
$rslt = db_set_row("test_type", 1, $X);
var_dump($rslt);
 */
?>

<?php
require_once "dbconn.php";
function mod_row(
  $tbl,
  $kvs,
  $where_clause
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $sql  = "update $tbl set ";
  $first = true;
  $save_kvs = $kvs;
  foreach ( $kvs as $key => $val ) {
    if ( is_null($val) ) { 
      continue; 
    }
    if ( $val === "__NULL__" ) { 
      $val = null;
    }
    if ( $first ) { 
      $first = false;
    }
    else {
      $sql .= ", ";
    }
    $sql .= " $key = :$key ";
    $X[$key] = $val;
  }
  $sql  .= $where_clause;
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute($X); 
  if ( !$rslt ) { 
    var_dump($sql); var_dump($X);
    rs_assert(false, "mod_row failed");
  }
  return true;
}
/*
$tbl = "variant";
$X['custom_data'] = "__NULL__";
$X['description'] = "some description data";
mod_row($tbl, $X, "where id = 2");
 */
?>

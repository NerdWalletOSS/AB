<?php
require_once "dbconn.php";
require_once "rs_assert.php";
require_once "db_num_rows.php";

function mod_cell(
  $tbl,
  $field,
  $val,
  $where_clause 
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }
  $nR = db_num_rows($tbl, $where_clause);
  rs_assert($nR == 1 );

  $sql  = "update $tbl set $field = :$field ";
  $sql .= " where $where_clause ";
  $X[$field] = $val;
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute($X); rs_assert($rslt );
  return true;
}
/*
mod_cell("test", "description", NULL, " id = 1 ");
 */
?>

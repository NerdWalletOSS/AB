<?php
require_once "bye.php";
require_once "dbconn.php";

function db_get_variant(
  $test_id,
  $name
)
{
  $dbh = dbconn(); assert($dbh);
  //-----------------------------------
  $sql  = "select * from variant where ";
  $sql .= "test_id = :test_id and ";
  $sql .= "name      = :name ";
  $stmt = $dbh->prepare($sql);
  $X['test_id'] = $test_id;
  $X['name']      = $name;
  $rslt = $stmt->execute($X); if ( !$rslt ) { return null; }
  $nR = $stmt->rowCount();
  assert($nR == 1);
  $Y = $stmt->fetch(PDO::FETCH_ASSOC);
  return $Y;
}
/*
  $test_id = $argv[1];
  $X = db_get_variant($test_id, "Control");
  var_dump($X);
*/
?>

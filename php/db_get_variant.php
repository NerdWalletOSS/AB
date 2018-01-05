<?php
require_once "bye.php";
require_once "dbconn.php";

function db_get_variant(
  $abtest_id,
  $name
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $sql  = "select * from variant where ";
  $sql .= "abtest_id = :abtest_id and ";
  $sql .= "name      = :name ";
  $stmt = $dbh->prepare($sql);
  $X['abtest_id'] = $abtest_id;
  $X['name']      = $name;
  $rslt = $stmt->execute($X); if ( !$rslt ) { return null; }
  $nR = $stmt->rowCount();
  if ( $nR != 1 ) { return null; }
  $Y = $stmt->fetch(PDO::FETCH_ASSOC);
  return $Y;
}
if ( getenv("ENABLE_PHP_CLI") != "" ) { 
  $abtest_id = $argv[1];
  $X = db_get_variant($abtest_id, "Control");
  var_dump($X);
}
?>

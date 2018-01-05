<?php
require_once "bye.php";
require_once "dbconn.php";

function db_get_test(
  $test_id
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $sql  = "select * from abtest where pk = :test_id";
  $stmt = $dbh->prepare($sql);
  $X['test_id'] = $test_id;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  if ( $nR >  1 ) { go_BYE(""); }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row;
}
function alt_db_get_test(
  $test_name,
  $test_type
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $rslt = db_get_row("test_type", "name", $test_type);
  if ( !$rslt ) { 
    $GLOBALS["err"] = "Invalid test type $test_type";  return false; 
  }
  $test_type_id = $rslt['pk']; 
  //-----------------------------------
  $sql  = "select * from abtest where name = :name ";
  $sql .= " and test_type_id = :test_type_id ";
  $stmt = $dbh->prepare($sql);
  $X['name']         = $test_name;
  $X['test_type_id'] = $test_type_id;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  if ( $nR >  1 ) { go_BYE(""); }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row;
}
?>

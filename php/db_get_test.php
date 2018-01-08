<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "lkp.php";

function db_get_test(
  $test_id,
  $test_name,
  $test_type
)
{
  $dbh = dbconn();  assert(isset($dbh));
  $archived_id = lkp("state", "archived");
  assert(isset($archived_id));
  if ( isset($test_id) ) { 
    $sql  = "select * from test where pk = :test_id";
    $X['test_id'] = $test_id;
  }
  else {
    $test_type_id = lkp("test_type", $test_type);
    $sql  = "select * from test where name = :name ";
    $sql .= " and test_type_id = :test_type_id ";
    $X['name']         = $test_name;
    $X['test_type_id'] = $test_type_id;
  }
  $sql .= " and state_id != $archived_id ";
  $stmt = $dbh->prepare($sql);
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  assert($nR <= 1 );
  if ( $nR == 0 ) { return null; }
  $row = $stmt->fetch(PDO::FETCH_ASSOC);
  return $row;
}
?>

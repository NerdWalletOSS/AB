<?php
require_once "bye.php";
require_once "dbconn.php";
function db_get_ramp_history(
  $abtest_id
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $sql  = "select * from ramp_history where abtest_id = :abtest_id ";
  $sql .= "order by when_changed desc ";
  $stmt = $dbh->prepare($sql);
  $X['abtest_id'] = $abtest_id;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("select failed"); }
  $nR = $stmt->rowCount();
  if ( $nR == 0 ) { return null; }
  $X = array($nR);
  for ( $i = 0; $i < $nR; $i++ ) {
    $X[$i] = $stmt->fetch(PDO::FETCH_ASSOC);
  }
  return $X;
}
?>

<?php
require_once "bye.php";
require_once "dbconn.php";

function db_get_percentage(
  $variant_id
)
{
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  //-----------------------------------
  $sql  = "select * from ramp_history where variant_id = :variant_id ";
  $sql .= "order by t_change asc";
  $stmt = $dbh->prepare($sql);
  $X['variant_id'] = $variant_id;
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

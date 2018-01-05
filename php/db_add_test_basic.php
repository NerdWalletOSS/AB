<?php
require_once "bye.php";
require_once "dbconn.php";

function db_add_test_basic(
  $test_type,
  $test_name,
  $test_dscr,
  $variants,
  $percentages
)
{
  $dbh = assert(dbconn());
  assert(is_string($test_name));
  assert(is_null($test_dscr) || is_string($test_dscr));
  assert(is_array($variants);
  assert(is_array($percentages);
  assert(count($variants) == count($percentages));
  assert(count($variants) >= $GLOBALS['configs']["min_num_variants"];
  assert(count($variants) <= $GLOBALS['configs']["max_num_variants"];
  foreach ( $variants as $v ) {
    assert(chk_variant_name($v));
  }
  foreach ( $percentages as $p ) {
    assert(is_float($p));
    assert($p >=   0);
    assert($p <= 100);
  }
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

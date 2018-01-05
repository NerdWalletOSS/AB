<?php
require_once "db_get_rows.php";
function db_get_counts(
  $test_name,
  $test_type
)
{
  $R = db_get_row("test_type", "name", $test_type);
  $test_type_id = $R['pk']; 
  $rslt = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id" ); 
  if ( !$rslt ) { return null; }
  $abtest_id = $rslt['pk'];
  $variants = db_get_variants($abtest_id);
  $nV = count($variants);
  for ( $i = 0; $i < $nV; $i++ ) { 
    $v = $variants[$i];
    $name  = $v['name'];
    $count = $v['count'];
    $V[$name] = $count; 
  }
  return $V;
}
?>

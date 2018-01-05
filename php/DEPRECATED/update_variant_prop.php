<?php
// ABTEST
require_once "bye.php";
require_once "dbconn.php";

function update_variant_prop(
  $abtest_id,
  $variant,
  $percentage
)
{
  $dbh = dbconn();  if ( !$dbh ) { go_BYE(""); }

  $sql  = "update variant set percentage = :percentage ";
  $sql .= " where abtest_id = :abtest_id  and ";
  $sql .= " name = :name ";
  $stmt = $dbh->prepare($sql);
  $X['percentage'] = $percentage;
  $X['abtest_id']  = $abtest_id;
  $X['name']       = $variant;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("update failed"); }

  unset($X); unset($sql); unset($stmt);
  $sql  = "update variant set count = 0 ";
  $sql .= " where abtest_id = :abtest_id  and ";
  $sql .= " name = :name ";
  $stmt = $dbh->prepare($sql);
  $X['abtest_id']  = $abtest_id;
  $X['name']       = $variant;
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("update failed"); }
  return true;
}
function update_variant_prop_in_rh(
  $abtest_id,
  $V,
  $var_name,
  $var_perc,
  $d_change,
  $t_change
)
{
  $dbh = dbconn();  if ( !$dbh ) { go_BYE(""); }
  //--------------------------------------
  $sql  = "update ramp_history set "; 
  $sql .= " percentage = :percentage, ";
  $sql .= " when_changed = :d_change, ";
  $sql .= " t_change = :t_change ";
  $sql .= " where abtest_id = :abtest_id ";
  $sql .= " and variant_id = :variant_id ";
  $sql .= " and ramp_num = :ramp_num ";
  //--------------------------------------
  $stmt = $dbh->prepare($sql);
  $nV = count($V);
  if ( count($var_name) != $nV ) { go_BYE(""); }
  if ( count($var_name) != $nV ) { go_BYE(""); }
  if ( count($var_perc) != $nV ) { go_BYE(""); }
  for ( $i = 0; $i < $nV; $i++ ) { 
    unset($X);
    $variant_id =  0;
    $percentage = -1;
    $name = "";
    foreach ( $V as $v ) { 
      if ( $v['name'] == $var_name[$i] ) {
        $variant_id = $v['pk']; 
        $percentage = $var_perc[$i];
        break;
      }
    }
    if ( $variant_id == 0 )  { go_BYE(""); } 
    $X['variant_id'] = $variant_id;
    $X['percentage'] = $percentage;
    $X['abtest_id' ] = $abtest_id;
    $X['ramp_num']   = 1;
    $X['d_change']   = $d_change;
    $X['t_change']   = $t_change;
    $X['abtest_id']  = $abtest_id;
    $rslt = $stmt->execute($X); if ( !$rslt ) { 
      var_dump($sql);
      var_dump($stmt);
      var_dump($X);
      go_BYE("update failed"); }
  }
  return true;
}
?>

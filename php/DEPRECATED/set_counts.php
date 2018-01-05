<?php
// SET COUNT
require_once "bye.php";
require_once "dbconn.php";

function set_counts(
  $abtest_id,
  $ramp_num,
  $new_counts /* JSON: array of counts */
)
{
  $dbh = dbconn();  
  if ( !$dbh ) { 
    go_BYE("", false); 
  }

  $sql  = "update variant set count = :count ";
  $sql .= "where abtest_id = :abtest_id and name = :name ";
  $stmt = $dbh->prepare($sql);

  $sql2  = "update ramp_history set count = :count where ";
  $sql2 .= "variant_id = :variant_id and ramp_num = :ramp_num";
  $stmt2 = $dbh->prepare($sql2);

  if ( ( is_null($abtest_id) ) || ( !is_numeric($abtest_id) ) ) { 
    debug_print_backtrace(); return false; 
  }
  if ( is_null($new_counts) ) { 
    debug_print_backtrace(); return false; 
  }
  $X = json_decode($new_counts); 
  if ( !$X ) { 
    debug_print_backtrace(); return false; 
  }
  //-----------------------------------------------------------
  $nX = count($X);
  foreach ( $X as $variant => $count ) { 
    unset($Y); unset($Y2); 
    $Y['count']     = $count; 
    $Y['abtest_id'] = $abtest_id; 
    $Y['name']      = $variant; 

    $v = db_get_row("variant", "name", $variant, 
      " and abtest_id = $abtest_id");
    if ( !$v ) { debug_print_backtrace(); return false; }
    $variant_id = $v['pk']; 
    $rslt = $stmt->execute($Y); 
    if ( !$rslt ) { 
      debug_print_backtrace(); return false; 
    }
    $Y2['variant_id'] = $variant_id;
    $Y2['ramp_num'] = $ramp_num;
    $Y2['count']      = $count;
    $rslt2 = $stmt2->execute($Y2); 
    if ( !$rslt2 ) { 
      debug_print_backtrace(); return false; 
    }
  }
  return true;
}
?>

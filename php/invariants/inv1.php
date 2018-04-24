<?php
require_once 'dbconn.php';
require_once 'rs_assert.php';
function invariant_1(
)
{
  $dbh = dbconn();
  rs_assert($dbh);
  // Execute query. Say return value is $X

  foreach ( $X as $x ) {  
    $xid      = $x['external_id'];
    $pred_xid = $x['pred_external_id'];
    $pred_id = $x['pred_id'];
    if ( !empty($pred_id) ) { 
      rs_assert($xid == $pred_xid, "Error on test $xid, $pred_id\n");
    }
  }
  return true;
}
?>

<?php
require_once 'db_get_rows.php';
require_once 'db_get_row.php';
require_once 'lkp.php';

function find_tests_to_follow(
  $channel
)
{
  rs_assert($channel);
  $channel_id  = lkp("channel", "$channel");
  rs_assert($channel_id);
  $archived_id = lkp("state", "archived");
  $R1 = db_get_rows("test", 
    " state_id = $archived_id and channel_id = $channel_id ");
  rs_assert($R1, "No tests to follow");
  $eligible_to_follow = array(); $eidx = 0;
  foreach ( $R1 as $r ) { 
    $tid = $r['id'];
    $x = db_get_row("test", "pred_id", $tid);
    if ( is_null($x) ) {
      $eligible_to_follow[$eidx++] = array(
        'id' => $r['id'],
        'name' => $r['name']
      );
    }
  }
  if ( $eidx == 0 ) { $eligible_to_follow = null; }
  return $eligible_to_follow;
}
/*
$x = find_tests_to_follow("facebook");
var_dump($x);
 */
?>

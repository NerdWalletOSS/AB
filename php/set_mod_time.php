<?php
require_once "bye.php";
require_once "dbconn.php";
function set_mod_time(
  $abtest_id,
  $mod_time
)
{
  $dbh = dbconn(); if ( !$dbh ) { exit("ERROR"); }
  $sql  = "update abtest set most_recent_update = :mod_time ";
  $sql .= "where pk = :abtest_id  ";
  $stmt = $dbh->prepare($sql);
  $X['mod_time']  = $mod_time;
  $X['abtest_id'] = $abtest_id;
  var_dump($sql);
  var_dump($X);
  $rslt = $stmt->execute($X); if ( !$rslt ) { go_BYE("update failed"); }
  return true;
}
if ( getenv("PHP_STANDALONE") ) {
  $dbh = dbconn();
  $abtest_id = 1;
  $mod_time = "2016-07-14 12:34:56";
  set_mod_time($abtest_id, $mod_time);
}
?>

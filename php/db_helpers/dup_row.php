<?php
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "insert_row.php";
require_once "mod_row.php";
function dup_row(
  $tbl,
  $id,
  $changes_to_old, /* changes to be made to old row */
  $changes_to_new /* changes to be made to new row */
)
{
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

  $R = db_get_row($tbl, "id", $id);
  if ( $changes_to_old ) { 
    print("changing old " . $R['id'] . " XXX \n");
    mod_row($tbl, $changes_to_old, " where id = " . $R['id']);
  }
  unset($R['id']);
  if ( $changes_to_new ) { 
    foreach  ( $changes_to_new as $k => $v ) {
      $R[$k] = $v;
    }
  }
  $new_id = insert_row($tbl, $R);
  return $new_id;
}
/* Quick and dirty test 
$changes_to_new = array( "name" => "XXXX" );
$changes_to_old = array( "name" => "YYYY" );
$new_id = dup_row("config", 1, $changes_to_old, $changes_to_new);
var_dump($new_id);
 */
?>

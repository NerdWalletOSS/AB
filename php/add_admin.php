<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "aux_check_name.php";
require_once "load_configs.php";
require_once "add_admin_channel.php";

function add_admin(
  $name
)
{
  $GLOBALS["err"] = "";
  if ( !add_admin_channel("admin", $name) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  return true;
}
?>

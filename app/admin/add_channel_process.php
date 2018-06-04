<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");

require_once "add_channel.php";


//-----------------------------------------------------------
$channel = $_GET['channel'];

$rslt = add_channel($channel);
var_dump($rslt);
if ( !$rslt ) { 
  header('Location: admin.php?msg='.urlencode($GLOBALS["err"]));
}
else {
  header('Location: admin.php?msg='.urlencode($channel).' Added as a new Channel');
}
?>

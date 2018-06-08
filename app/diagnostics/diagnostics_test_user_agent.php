<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "chk_url.php";

//-----------------------------------------------------------
$url = $_GET['url'];

$rslt = chk_url( $url );

if ( !$rslt ) { 
  header('Location: diagnostics.php?msg=BAD URL: '.$url.' '.$GLOBALS["err"]);
}
else {
  header('Location: diagnostics.php?msg=URL is OK: '.$url);
}
?>

<?php 
require_once "../../test/chk_url.php";

//-----------------------------------------------------------
$url = $_GET['url'];

$rslt = chk_url( $url );

if ( !$rslt ) { 
  header('Location: admin.php?msg=BAD URL: '.$url.' '.$GLOBALS["err"]);
}
else {
  header('Location: admin.php?msg=URL is OK: '.$url);
}
?>

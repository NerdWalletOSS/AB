<?php

set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");

#export database
if ( $argc != 2 ) { exit ("Usage is php ${argv[0]} <datadir>\n"); }
require_once "db2tb.php";
$datadir = $argv[1];
$rslt = db2tb($datadir);
if ( !$rslt ) { go_BYE(""); }
?>

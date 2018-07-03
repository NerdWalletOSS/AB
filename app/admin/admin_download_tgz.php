<?php
require_once "webapp_api.php";
#export database
require_once "../sql/exportdb2csv/db2tb.php";
require_once "../sql/exportdb2csv/tb2csv.php";

function recurseRmdir($dir) {
  $files = array_diff(scandir($dir), array('.','..'));
  foreach ($files as $file) {
    (is_dir("$dir/$file")) ? recurseRmdir("$dir/$file") : unlink("$dir/$file");
  }
  return rmdir($dir);
}


if (!is_dir('/home/scrabble/local/htdocs/AB/tcc/data')) {
  mkdir('/home/scrabble/local/htdocs/AB/tcc/data', 0777, true);
}
db2tb('/home/scrabble/local/htdocs/AB/tcc/data');

$filename = 'data-'. date("d-m-Y_(G_i_s)") .'.tar.gz';
$mime = "application/x-tgz";

header("Content-Type: " . $mime);
header('Content-Disposition: attachment; filename="' . $filename . '"');
$cmd = "tar -cz "."data";
passthru($cmd);
if (is_dir('/home/scrabble/local/htdocs/AB/tcc/data')) {
  recurseRmdir('/home/scrabble/local/htdocs/AB/tcc/data');
}
exit(0);

?>

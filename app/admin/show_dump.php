<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
//require_once "common/header_admin.php"; 
require_once "../../php/load_configs.php";
?>
<?php
ob_start();
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }

 
$username = "root"; 
$password = ""; 
$hostname = "localhost"; 
$dbname   = "abdb2";
 
// if mysqldump is on the system path you do not need to specify the full path
// simply use "mysqldump --add-drop-table ..." in this case
/*
$command = "mysqldump --add-drop-table --host=$hostname
    --user=$username ";
if ($password) 
        $command.= "--password=". $password ." "; 
$command.= $dbname;
*/
$command = "mysqldump -u $username $dbname ";
system($command);
 
$dump = ob_get_contents(); 
ob_end_clean();
 
// send dump file to the output
header('Content-Description: File Transfer');
header('Content-Type: application/octet-stream');
header('Content-Disposition: attachment; filename='.basename($dbname . "_" . 
    date("Y-m-d_H-i-s").".sql"));
flush();
echo $dump;
exit();

?>


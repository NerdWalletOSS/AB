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
header("Content-type: text/plain");
header('Content-Disposition: attachment; filename="abdb2.sql"');
  $dbh = dbconn();
  if ( !$dbh ) { go_BYE(""); }
 //put table names you want backed up in this array.
//leave empty to do all
$tables = array();

backup_tables($dbh, $tables);

function backup_tables($DBH, $tables) {

$DBH->setAttribute(PDO::ATTR_ORACLE_NULLS, PDO::NULL_TO_STRING );

//Script Variables
$compression = false;
$BACKUP_PATH = "";
$nowtimename = time();


/*create/open files
if ($compression) {
$zp = gzopen($BACKUP_PATH.$nowtimename.'.sql.gz', "w9");
} else {
$handle = fopen($BACKUP_PATH.$nowtimename.'.sql','a+');
}
*/

$return="";
// DROP DATABASE abdb2; || true
$return.= 'DROP DATABASE abdb2;';
$return.= 'CREATE DATABASE abdb2;';
$return.= 'USE abdb2;';
$return.= "
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;";

//array of all database field types which just take numbers 
$numtypes=array('tinyint','smallint','mediumint','int','bigint','float','double','decimal','real');

//get all of the tables
if(empty($tables)) {
$pstm1 = $DBH->query('SHOW TABLES');
while ($row = $pstm1->fetch(PDO::FETCH_NUM)) {
$tables[] = $row[0];
}
} else {
$tables = is_array($tables) ? $tables : explode(',',$tables);
}

//cycle through the table(s)

foreach($tables as $table) {
$result = $DBH->query('SELECT * FROM '.$table);
$num_fields = $result->columnCount();
$num_rows = $result->rowCount();

//uncomment below if you want 'DROP TABLE IF EXISTS' displayed
$return.= 'DROP TABLE IF EXISTS `'.$table.'`;'; 


//table structure
$pstm2 = $DBH->query('SHOW CREATE TABLE '.$table);
$row2 = $pstm2->fetch(PDO::FETCH_NUM);
$ifnotexists = str_replace('CREATE TABLE', 'CREATE TABLE ', $row2[1]);
$return.= $ifnotexists.';';
$return.= 'LOCK TABLES '.$table.' WRITE;';
$return.= '/*!40000 ALTER TABLE `abtest` DISABLE KEYS */;';

/*
if ($compression) {
gzwrite($zp, $return);
} else {
fwrite($handle,$return);
}
*/
echo $return;
$return = "";

//insert values
if ($num_rows){
$return= 'INSERT INTO `'.$table."` (";
$pstm3 = $DBH->query('SHOW COLUMNS FROM '.$table);
$count = 0;
$type = array();

while ($rows = $pstm3->fetch(PDO::FETCH_NUM)) {

if (stripos($rows[1], '(')) {$type[$table][] = stristr($rows[1], '(', true);
} else $type[$table][] = $rows[1];

$return.= $rows[0];
$count++;
if ($count < ($pstm3->rowCount())) {
$return.= ', ';
}
}

$return.= ')'.' VALUES';
/*
if ($compression) {
gzwrite($zp, $return);
} else {
fwrite($handle,$return);
}*/
echo $return;
$return = "";
}

while($row = $result->fetch(PDO::FETCH_NUM)) {
$return= '(';
for($j=0; $j<$num_fields; $j++) {
$row[$j] = addslashes($row[$j]);
//$row[$j] = preg_replace("\n","\\n",$row[$j]);


if (isset($row[$j])) {
//if number, take away "". else leave as string
if (in_array($type[$table][$j], $numtypes)) $return.= $row[$j] ; else $return.= "'".$row[$j]."'" ;
} else {
$return.= "''";
}
if ($j<($num_fields-1)) {
$return.= ',';
}
}
$count++;
if ($count < ($result->rowCount())) {
$return.= '),';
} else {
$return.= ');';

}
/*
if ($compression) {
gzwrite($zp, $return);
} else {
fwrite($handle,$return);
}*/
echo $return;
$return = "";
}
//$return="\n\n-- ------------------------------------------------ \n\n";
/*
if ($compression) {
gzwrite($zp, $return);
} else {
fwrite($handle,$return);
}*/
echo $return;
$return = "";
}



$error1= $pstm2->errorInfo();
$error2= $pstm3->errorInfo();
$error3= $result->errorInfo();
echo $error1[2];
echo $error2[2];
echo $error3[2];
/*
if ($compression) {
gzclose($zp);
} else {
fclose($handle);
}
*/
echo $return;
}

?>

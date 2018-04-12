<?php
require_once 'bye.php';
function extract_sql_params(
      $conf_file = "/opt/abadmin/db2.json",
      &$dbuser, 
      &$dbpass, 
      &$dbhost, 
      &$dbname,
      &$dbport
)
{
  if ( !is_file($conf_file) ) { 
    // Use defaults
    $dbuser     = "root";
    $dbpass = '';
    $dbhost = "localhost";
    $dbname = "abdb2";
  }
  else {
    $X = json_decode(file_get_contents($conf_file));
    if ( !$X ) { go_BYE(""); }
    $dbuser = $X->{"dbuser"};
    if ( ( !isset($dbuser) ) || ( $dbuser == "" ) ) { return null; }
    $dbpass = $X->{"dbpass"};
    $dbhost = $X->{"dbhost"};
    if ( ( !isset($dbhost) ) || ( $dbhost == "" ) ) { return null; }
    $dbname = $X->{"dbname"};
    if ( ( !isset($dbname) ) || ( $dbname == "" ) ) { return null; }
    if ( isset($X->{"dbport"} ) ) {
      $dbport = $X->{"dbport"};
      if ( ( $dbport == "" ) || ( $dbport <= 0 ) )  return null; 
    }
  }
  return true;
}

function dbconn(
  $conf_file = "/opt/abadmin/db2.json"
)
{
  if ( !is_file($conf_file) ) { 
    return null; 
  }
  if ( isset($_GLOBALS['DBH']) ) {
    $dbh = $GLOBALS['DBH'];
  }
  else {
    if ( !(extract_sql_params($conf_file, $dbuser, $dbpass, 
      $dbhost, $dbname, $dbport)) ) {
      return null;
    }
    $dbh = new PDO("mysql:host=$dbhost;dbname=$dbname;port=$dbport;charset=utf8", 
      $dbuser, $dbpass);
    if ( !$dbh ) { go_BYE(""); }
    $GLOBALS['DBH'] = $dbh;
  }
  return $dbh;  
}
/*
  require_once "bye.php";
  // $X = dbconn("reco_db.json");
  $X = dbconn();
  var_dump($X);
 */

  /*
   * I got the following error which got dixed when I used
   * 127.0.0.1 instead of localhost in dbhost
atal error: Uncaught PDOException: SQLSTATE[HY000] [2002] No such file or directory in /home/subramon/WORK/OSS/AB/php/db_helpers/dbconn.php:57
Stack trace:
#0 /home/subramon/WORK/OSS/AB/php/db_helpers/dbconn.php(57): PDO->__construct('mysql:host=loca...', 'root', '')
#1 /home/subramon/WORK/OSS/AB/php/db_helpers/dbconn.php(66): dbconn()
#2 {main}
  thrown in /home/subramon/WORK/OSS/AB/php/db_helpers/dbconn.php on line 57
*/

?>

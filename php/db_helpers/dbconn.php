<?php
require_once 'bye.php';
function extract_sql_params(
      $conf_file = "/opt/abadmin/db.json",
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
    $dbname = "abdb";
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
  $conf_file = "/opt/abadmin/db.json"
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
?>

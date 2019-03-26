<?php
<<<<<<< HEAD
=======
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
?>
>>>>>>> dev

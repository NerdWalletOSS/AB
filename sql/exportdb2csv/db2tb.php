<?php

set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
require_once "load_configs.php";
require_once "tb2csv.php";

# Find tables of a database
function db2tb(
  $datadir
) 
{
  // Load configs to see if we need to talk to RTS 
  if ( !load_configs() ) { 
    $GLOBALS["err"] = "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $dbhost  = $configs['dbhost'];
  $dbname  = $configs['dbname'];
  $dbuser  = $configs['dbuser'];
  $dbpass  = $configs['dbpass'];

  // TODO: P1 Why are we establishing this connection at all?
  $connection = mysql_connect($dbhost, $dbuser, $dbpass);
  if (!$connection ) { return false; }
  $selectdb = mysql_select_db($dbname) or 
    die("Database $dbname could not be selected");	
  $result = mysql_select_db($dbname)
    or die("database $dbname cannot be selected <br>");

  $sql = "SHOW TABLES FROM $dbname";
  $result = mysql_query($sql);

  if (!$result) {
    echo 'MySQL Error: ' . mysql_error();
    go_BYE( "DB Error, could not list tables\n");
  }
  while ( $row = mysql_fetch_row($result) ) {
    #echo "Table: {$row[0]}\n";
    $table = trim($row[0]);
    $rslt = tb2csv($datadir, $table);	
    if ( !$rslt ) { 
      echo "ERR: FILE:" . __FILE__ . " LINE: " . __LINE__ . "\n";
      return false;
    }
  }
  return true;
}
?>

<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
function set_description(
  $tbl,
  $description = "",
  $pk
)
{
  if ( ( $tbl == "abtest" ) || ( $tbl == "variant" ) ) { 
    // all is well
  }
  else {
    $GLOBALS["err"] = "Bad parameters to " . __FILE__; return false; 
  }
  if ( $pk <= 0 )  {
    $GLOBALS["err"] = "Bad parameters to " . __FILE__; return false; 
  }
  //--------------------------------------------------------
  $x = db_get_row($tbl, "pk", $pk);
  if ( !$x ) { 
    $GLOBALS["err"] = "Bad parameters to " . __FILE__; return false; 
  }
  //--------------------------------------------------------
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] = "Cannot connect to database"; return false; 
  }
  // An update to description doe NOT cause change to 
  // most_recent_update or t_update
  $sql  = "update $tbl set description = :description ";
  $sql .= "where pk = :pk ";
  $stmt = $dbh->prepare($sql);

  $X['description'] = $description;
  $X['pk']          = $pk;
  $rslt = $stmt->execute($X); 
  if ( !$rslt ) { print_r($stmt->errorInfo()); return false; }
  return true;
}
/*
if ( getenv("ENABLE_PHP_CLI") != "" ) {
  if ( $argc != 4 ) { exit("Need 3 arguments to ". $argv[0] . "\n"); }
  set_description($argv[1], $argv[2], $argv[3]);
  if ( isset($GLOBALS["err"]) ) { print_r($GLOBALS["err"]); }
  // php set_description.php test 1 XYZ
  // php set_description.php variant 1 XYZ
}
 */
?>

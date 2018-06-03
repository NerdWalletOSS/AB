<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "aux_chk_name.php";
require_once "load_configs.php";
error_reporting(E_NOTICE);

function add_admin_channel(
  $tbl,
  $name
)
{
  $GLOBALS['err'] = ""; //TO DELETE
  $dbh = dbconn(); 
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['configs'];
  $key = "max_len_" . $tbl . "_name";
  $max_len = $configs[$key];
  //----------------------------------------------
  $name = trim(strtolower($name));
  if ( $name == "" ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Name is null "; return false; 
  }
  //-----------------------------------------------
  if ( !aux_chk_name($name) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "$tbl Name restricted to alphanumeric, underscore";
    return false;
  }
  //-----------------------------------------------
  $rslt = db_get_row($tbl, "name", $name); 
  if ( !is_null($rslt) ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Entry $name already exists."; return false; 
  }
  //-----------------------------------------------
  if ( strlen($name)  > $max_len ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Name too long [$name]"; return false; 
  }
  //-----------------------------------
  $sql  = "insert into $tbl values (NULL, :name, 0)";
  $stmt = $dbh->prepare($sql);
  $X['name'] = $name;
  var_dump($sql);
  var_dump($X);
  // Start transaction
  try {
    $dbh->beginTransaction();
    $rslt = $stmt->execute($X); 
    if ( !$rslt ) { throw new Exception("stmt $stmt failed");}
    $dbh->commit();
  } catch ( PDOException $ex ) {
    var_dump($ex);
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  return true;
}
// For quick and dirty testing
// add_admin_channel("admin", "ramesh");
?>

<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "aux_check_name.php";
require_once "load_configs.php";

function add_admin_channel(
  $tbl,
  $name
)
{
  $dbh = dbconn(); 
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $key = "max_len_" . $tbl . "_name";
  $max_len = $configs[$key];
  //----------------------------------------------
  $name = trim(strtolower($name));
  if ( $name == "" ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Name is null "; return false; 
  }
  //-----------------------------------------------
  if ( !aux_check_name($name) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "$tbl Name restricted to alphanumeric, underscore";
    return false;
  }
  //-----------------------------------------------
  $rslt = db_get_row($tbl, "name", $name); 
  if ( !is_null($rslt) ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "User $name already exists."; return false; 
  }
  //-----------------------------------------------
  if ( strlen($name)  > $max_len ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Name too long [$name]"; return false; 
  }
  //-----------------------------------
  $sql  = "insert into $tbl values (NULL, :name)";
  $stmt = $dbh->prepare($sql);
  $X['name'] = $name;
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
?>

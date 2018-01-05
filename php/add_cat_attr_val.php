<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "load_configs.php";
require_once "db_get_row.php";

function add_cat_attr_val(
  $attr_name,
  $attr_val,
  $attr_val_dscr
)
{
  $GLOBALS['err'] = "";
  load_configs();
  $configs = $GLOBALS['CONFIGS'];
  $dbh = dbconn();  
  // START: Error checking on inputs 
  $attr_val = strtolower(trim($attr_val));
  $len = strlen($attr_val);
  if ( ( $len < 1 ) || ( $len > $configs['max_len_cat_attr_val'] ) ) { 
    $GLOBALS["err"] .= "Categorical Attribute length bad [$attr_val]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  if ( !aux_check_name($attr_val) ) { 
    $GLOBALS["err"] .= "Attribute value has bad characters [$attr_val]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //---------------------------------------------------------
  $attr_name = strtolower(trim($attr_name));
  $X = db_get_row("cat_attr", "name", $attr_name);
  if ( !$X ) { 
    $GLOBALS["err"] .= "Categorical Attribute [$attr_name] missing\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $cat_attr_id = $X['pk'];
  //---------------------------------------------------------
  $X = db_get_row("cat_attr_val", "cat_attr_id", $cat_attr_id, " and name = '$attr_val' ");
  if ( $X ) { 
    $GLOBALS["err"] .= "Value [$attr_val] for Categorical Attribute [$attr_name] exists\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //  STOP: Error checking on inputs 
  //---------------------------------------------
  // --- Prepare query for Insert into cat_attr_val
  $sql1  = "insert into cat_attr_val ";
  $sql1 .= "(cat_attr_id, name, description) values ";
  $sql1 .= "(:cat_attr_id, :name, :description) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['cat_attr_id']  = $cat_attr_id;
  $X1['name']         = $attr_val;
  $X1['description']  = $attr_val_dscr;
  try {
    $dbh->beginTransaction();
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { throw new Exception("stmt1 = $sql1 failed"); }
    $dbh->commit();
  } catch(PDOException $ex) {
    $dbh->rollBack();
    var_dump($ex);
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  return true;
}
/*
$X = add_cat_attr_val('Platform', 'Desktop', 'Desktop Applications');
var_dump($X);
$X = add_cat_attr_val('Platform', 'Desktop', 'Desktop Applications');
var_dump($X);
$X = add_cat_attr_val('Platform', 'Mobile', 'Junk Applications');
var_dump($X);
$X = add_cat_attr_val('Source', 'Paid', 'Paid Junk ');
var_dump($X);
$X = add_cat_attr_val('Source', 'Organic', 'Organic Junk ');
var_dump($X);
 */
?>

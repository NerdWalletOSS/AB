<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "load_configs.php";
require_once "db_num_rows.php";

function add_cat_attr(
  $attr_name,
  $attr_dscr
)
{
  $GLOBALS['err'] = "";
  load_configs();
  $configs = $GLOBALS['CONFIGS'];
  $dbh = dbconn();  
  // START: Error checking on inputs 
  $attr_name = strtolower(trim(($attr_name)));
  $len = strlen($attr_name);
  if ( ( $len < 1 ) || ( $len > $configs['max_len_cat_attr'] ) ) { 
    $GLOBALS["err"] .= "Categorical Attribute length bad [$attr_name]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  if ( !aux_check_name($attr_name) ) { 
    $GLOBALS["err"] .= "Attribute has bad characters [$attr_name]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //-----------------------------------------------------------------
  $nR = db_num_rows("cat_attr", " name = '$attr_name'");
  if ( $nR > 0 ) { 
    $GLOBALS["err"] .= "Categorical Attribute [$attr_name] exists\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //---------------------------------------------------------------------
  //  STOP: Error checking on inputs 
  //---------------------------------------------
  // --- Prepare query for Insert into cat_attr
  $sql1  = "insert into cat_attr ";
  $sql1 .= "(name, description) values ";
  $sql1 .= "(:name, :description) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['name']         = $attr_name;
  $X1['description']  = $attr_dscr;
  $attr_id = 0;
  try {
    $dbh->beginTransaction();
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { throw new Exception("stmt1 = $stmt1 failed"); }
    $dbh->commit();
    $attr_id = $dbh->lastInsertId();
  } catch(PDOException $ex) {
    $dbh->rollBack();
    var_dump($ex);
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------------
  return true;
}
/*
$X = add_cat_attr('ABC', 'abc def');
var_dump($X);
$X = add_cat_attr('DEF', 'def ghi');
var_dump($X);
 */
?>

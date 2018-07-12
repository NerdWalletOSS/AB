<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "load_configs.php";
require_once "db_get_row.php";
require_once "name_to_id.php";

function add_cat_attr_val_test(
  $attr_name,
  $attr_val,
  $test_name
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
    $GLOBALS["err"] .= "Categorical Attribute Value length bad [$attr_val]\n";
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
  if ( !$X ) { 
    $GLOBALS["err"] .= "Value [$attr_val] for Categorical Attribute [$attr_name] does not exist\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $cat_attr_val_id = $X['pk'];
  //-----------------------------------------------------
  $dormant_id = name_to_id("state", "name", "dormant");
  $test_type_id = name_to_id("test_type", "name", "ABTest");
  $X = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id and state_id = $dormant_id ");
  if ( !$X ) { 
    $GLOBALS["err"] .= "Test [$test_name] does not exist\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $abtest_id = $X['pk'];
  //-----------------------------------------------------------
  $X = db_get_row("cat_attr_val_test", "abtest_id", $abtest_id, 
    " and cat_attr_val_id = $cat_attr_val_id ");
  if ( $X ) { 
    $GLOBALS["err"] .= "This val_test already set for this test \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //  STOP: Error checking on inputs 
  //---------------------------------------------
  // --- Prepare query for Insert into cat_attr_val
  $sql1  = "insert into cat_attr_val_test ";
  $sql1 .= "(cat_attr_id, cat_attr_val_id, abtest_id) values ";
  $sql1 .= "(:cat_attr_id, :cat_attr_val_id, :abtest_id) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['cat_attr_id']     = $cat_attr_id;
  $X1['cat_attr_val_id'] = $cat_attr_val_id;
  $X1['abtest_id']       = $abtest_id;
  try {
    $dbh->beginTransaction();
    $rslt1 = $stmt1->execute($X1); 
    if ( !$rslt1 ) { throw new Exception("stmt1 $stmt1 failed"); }
    $dbh->commit();
  } catch(PDOException $ex) {
    var_dump($ex);
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  return true;
}
/*
$t = "c9LMmiRGsBK76SJGcWHt6GajYwTNribPargj7fMaMcxjr48LfRc8FDC2WQcD2FN";
$X = add_cat_attr_filter('Platform', 'Desktop', $t);
var_dump($X);
var_dump($GLOBALS['err']);
$X = add_cat_attr_filter('Platform', 'Desktop', $t);
var_dump($X);
$X = add_cat_attr_filter('Platform', 'Mobile', $t);
var_dump($X);
$X = add_cat_attr_filter('Source', 'Paid', $t);
var_dump($X);
 */
?>

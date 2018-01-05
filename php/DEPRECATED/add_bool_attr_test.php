<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "load_configs.php";
require_once "db_get_row.php";
require_once "yurl.php";

function add_bool_attr_test(
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
  switch ( $attr_val ) {
  case "false" : $bool_val = 0; break;
  case "true"  : $bool_val = 1; break;
  default : 
    $GLOBALS["err"] .= "Boolean Attribute Value is true or false\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
    break;
  }

  //---------------------------------------------------------
  $attr_name = strtolower(trim($attr_name));
  $X = db_get_row("bool_attr", "name", $attr_name);
  if ( !$X ) { 
    $GLOBALS["err"] .= "Boolean Attribute [$attr_name] missing\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $cat_attr_id = $X['pk'];
  //---------------------------------------------------------
  $test_type_id = name_to_id("test_type", "name", "ABTest");
  $X = db_get_row("abtest", "name", $test_name, " and test_type_id = $test_type_id ");
  if ( !$X ) { 
    $GLOBALS["err"] .= "Test [$test_name] does not exist\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $abtest_id = $X['pk'];
  //-----------------------------------------------------------
  $X = db_get_row("bool_attr_test", "abtest_id", $abtest_id, 
    " and bool_attr_id = $bool_attr_id ");
  if ( $X ) { 
    $GLOBALS["err"] .= "This bool attr already set for this test \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  //  STOP: Error checking on inputs 
  //---------------------------------------------
  // --- Prepare query for Insert into bool_attr
  $sql1  = "insert into bool_attr_test ";
  $sql1 .= "(bool_attr_id, abtest_id, bool_val) values ";
  $sql1 .= "(:bool_attr_id, :abtest_id, :bool_val) ";
  $stmt1 = $dbh->prepare($sql1);
  $X1['bool_attr_id'] = $bool_attr_id;
  $X1['abtest_id']    = $abtest_id;
  $X1['bool_val']     = $bool_val;
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
  // Figure out whether an RTS is available 
  $SP = list_rts();
  if ( is_array($SP) ) {
    $http_code = 0; $rslt = "";
    $server = $SP[0]['server'];
    $port   = $SP[0]['port'];
    yurl($server, $port, 
      "SetBoolAttrTest?TestName=$test_name&Attribute=$attr_name&Value=$attr_val", $http_code, $rslt);
    if ( $http_code != 200 ) {
      $GLOBALS["err"] .= "ERR: RTS says $rslt \n";
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
  }
  else {
    $GLOBALS["err"] .= "WARN: No RTS specified\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
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

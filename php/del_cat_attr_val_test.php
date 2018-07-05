<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "load_configs.php";
require_once "db_get_row.php";
require_once "name_to_id.php";

function del_cat_attr_val_test(
  $test_name
)
{
  $GLOBALS['err'] = "";
  load_configs();
  $configs = $GLOBALS['CONFIGS'];
  $dbh = dbconn();  
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
  $sql1  = "delete from cat_attr_val_test where abtest_id = :abtest_id ";
  $stmt1 = $dbh->prepare($sql1);
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

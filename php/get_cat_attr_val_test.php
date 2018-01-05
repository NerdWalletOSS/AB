<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_get_rows.php";
require_once "name_to_id.php";

function get_cat_attr_val_test(
  $test_name,
  $test_id=0
)
{
  $GLOBALS['err'] = "";
  $dbh = dbconn();  
  //-----------------------------------------------------------
  $dormant_id = name_to_id("state", "name", "dormant");
  $test_type_id = name_to_id("test_type", "name", "ABTest");
  $test_name = trim($test_name);
  if ( $test_name == "" ) { 
    $X = db_get_row("abtest", "pk", $test_id);
  }
  else {
  $X = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id ");
  }
  if ( !$X ) { 
    $GLOBALS["err"] .= "Test [$test_name] does not exist\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $abtest_id = $X['pk'];
  //-----------------------------------------------------------
  $output = "";
  $X = db_get_rows("cat_attr_val_test", " abtest_id = $abtest_id");
  if ( count($X) > 0 ) { 
    $output  = " { \"TestName\" : \"$test_name\", \"Filters\" : [ ";
    $first = true;
    foreach ($X as $x ) { 
      $attr_name = id_to_name($x['cat_attr_id'], "cat_attr");
      $attr_val = id_to_name($x['cat_attr_val_id'], "cat_attr_val");
      if ( $first ) { 
        $first = false;
      }
      else {
        $output .= ", ";
      }
      $output  .= " { \"Attribute\" : \"$attr_name\", \"Value\" : \"$attr_val\" } ";


    }
    $output .= " ] }";
  }
  return $output;
}
/*
$x = get_cat_attr_val_test("", 2);
var_dump($GLOBALS['err']);
var_dump($x);
 */

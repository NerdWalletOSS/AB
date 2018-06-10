<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "lkp.php";
require_once "rs_assert.php";

function db_get_test(
  $test_id,
  $test_name="",
  $test_type=""
)
{
  // $dbh = dbconn();  rs_assert(isset($dbh));
  if ( is_null($test_id) ) { 
    rs_assert($test_name != "");
    rs_assert($test_type != "");
    $test_type_id = lkp("test_type", $test_type);
    $T = db_get_row("test", "name", $test_name,
      " and test_type_id = $test_type_id ");
    rs_assert($T);
    $test_id = $T['id'];
  }
  else {
    $T = db_get_row("test", "id", $test_id);
  }
  if ( is_null($T) ) { return null; }
  $T['Creator']  = lkp("admin",     $T['creator_id'],   "reverse");
  $T['TestType'] = lkp("test_type", $T['test_type_id'], "reverse");
  $T['BinType']  = lkp("bin_type",  $T['bin_type_id'],  "reverse");
  $T['State']    = lkp("state",     $T['state_id'],     "reverse");
  if ( isset($T['channel_id']) ) {
    $T['Channel']  = lkp("channel",   $T['channel_id'],   "reverse");
  }
  //------------------------------------
  $V = db_get_rows("variant", " test_id = $test_id and is_del = false");

  $nV = count($V);
  $Variants = array($nV);
  $vidx = 0;
  foreach ( $V as $v ) {
    $Variants[$vidx++] = $v;
  }
  $T['Variants'] = $Variants;
  //------------------------------------
  $DV = db_get_rows("device_x_variant", "test_id = $test_id ");
  if ( !is_null($DV) ) {
    $D  = $GLOBALS['device'];
    unset($xxx);
    foreach ( $D as $k => $v ) { 
      $xxx[$k] = db_get_rows("device_x_variant", 
          "test_id = $test_id and device_id = $v " );
    }
    $T['DeviceCrossVariant'] = $xxx;
  }
  //--- categorical attributes as filters
  $CAVT = db_get_rows("cat_attr_val_test", " test_id = $test_id ");
  if ( $CAVT ) { 
    $cavidx = 0;
    unset($xxx);
    foreach ( $CAVT as $cavt ) { 
      $id              = $cavt['id'];
      $attr_id         = $cavt['attr_id'];
      $attr            = lkp("attr", $attr_id, "reverse");
      $cat_attr_val_id = $cavt['cat_attr_val_id'];
      $test_id         = $cavt['test_id'];
      $is_on           = $cavt['is_on'];
      $val             = lkp("cat_attr_val", $cat_attr_val_id, "reverse");
      $xxx[$cavidx++] = array(
        "Attribute" => $attr, 
        "attr_id" => $attr_id, 
        "test_id" => $test_id, 
        "Value" => $val,
        "cat_attr_val_id" => $cat_attr_val_id,
        "id" => $id,
        "is_on" => $is_on,
      );
    }
    $T['CategoricalFilters'] = $xxx;
  }
  //--- numerical attributes as filters
  // TODO when we implement this feature. Not needed for now.
  // -------------------------------

  return $T;
}
/*
$x = db_get_test(1);
$y = json_encode($x);
print($y);
 */

?>

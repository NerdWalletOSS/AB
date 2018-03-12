<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "lkp.php";
require_once "rs_assert.php";

function db_get_test(
  $test_id
)
{
  // $dbh = dbconn();  rs_assert(isset($dbh));
  $T = db_get_row("test", "id", $test_id);
  if ( is_null($T) ) { return null; }
  $T['Creator']  = lkp("admin",     $T['creator_id'],   "reverse");
  $T['TestType'] = lkp("test_type", $T['test_type_id'], "reverse");
  $T['BinType']  = lkp("bin_type",  $T['bin_type_id'],  "reverse");
  $T['State']    = lkp("state",     $T['state_id'],     "reverse");
  if ( isset($T['channel_id']) ) {
    $T['Channel']  = lkp("channel",   $T['channel_id'],   "reverse");
  }
  //------------------------------------
  $V = db_get_rows("variant", " test_id = $test_id");

  $nV = count($V);
  $Variants = array($nV);
  $vidx = 0;
  foreach ( $V as $v ) {
    $Variants[$vidx++] = $v;
  }
  $T['Variants'] = $Variants;
  //------------------------------------
  $FC = db_get_rows("cat_attr_val_test", "test_id = $test_id");
  if ( !is_null($FC) ) {
    $T['CatAttrValTest'] = $FC;
    // TODO Needs to be completed
  }
  //------------------------------------
  $FB = db_get_rows("bool_attr_test", "test_id = $test_id");
  if ( !is_null($FB) ) {
    $T['BoolAttrTest'] = $FB;
    // TODO Needs to be completed
  }
  //------------------------------------
  $DV = db_get_rows("device_x_variant", "test_id = $test_id ");
  if ( !is_null($DV) ) {
    $D  = $GLOBALS['device'];
    $xxx = array(count($D));
    foreach ( $D as $k => $v ) { 
      $xxx[$k] = db_get_rows("device_x_variant", 
          "test_id = $test_id and device_id = $v " );
    }
    $T['DeviceCrossVariant'] = $xxx;
  }
  //--- boolean attributes as filters
  $B = db_get_rows("bool_attr_test", 
    " test_id = $test_id and is_on = true ");
  if ( $B ) { 
    $bidx = 0;
    unset($xxx);
    foreach ( $B as $b ) { 
      $attr_id = $b['attr_id'];
      $attr    = lkp("attr", $attr_id, "reverse");
      $val     = $b['val'];
      $xxx[$bidx] = array("Attribute" => $attr, "Value" => $val);
    }
    $T['BooleanFilters'] = $xxx;
  }
  $B = db_get_rows("cat_attr_val_test", 
    " test_id = $test_id and is_on = true ");
  if ( $B ) { 
    $bidx = 0;
    unset($xxx);
    foreach ( $B as $b ) { 
      $attr_id = $b['attr_id'];
      $attr    = lkp("attr", $attr_id, "reverse");
      $val     = $b['val'];
      $xxx[$bidx++] = array("Attribute" => $attr, "Value" => $val);
    }
    $T['CategoricalFilters'] = $xxx;
  }
  //--- categorical attributes as filters
  $CAV = db_get_rows("cat_attr_val_test", 
    " test_id = $test_id and is_on = false ");
  if ( $CAV ) { 
    $cavidx = 0;
    unset($xxx);
    foreach ( $CAV as $cav ) { 
      $attr_id = $cav['attr_id'];
      $attr    = lkp("attr", $attr_id, "reverse");
      $cat_attr_val_id = $cav['cat_attr_val_id'];
      $val     = lkp("cat_attr_val", $cat_attr_val_id, "reverse");
      $xxx[$cavidx++] = array("Attribute" => $attr, "Value" => $val);
    }
    $T['CategoricalFilters'] = $xxx;
  }
  //--- numerical attributes as filters
  // -------------------------------

  return $T;
}
/*
$x = db_get_test(1);
$y = json_encode($x);
print($y);
 */
?>

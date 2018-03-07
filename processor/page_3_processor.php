<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'db_get_test.php';
require_once 'set_device_specific_variant.php';
require_once "get_variant_idx.php";
require_once "db_get_rows.php";
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "InsertTest" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$test_id = $_POST['TestID'];
$TestType = $_POST['TestType'];
// SETTING NEW VALUES OF DEVICE X VARIANT
if (isset($_POST['is_dev_specific'])) {
$T['is_dev_specific'] = $_POST['is_dev_specific'];
}
$T = db_get_test($test_id);
$n_var = count($T['Variants']);
$device    = db_get_rows('device');
$nD = count($device);
for ( $i = 0; $i < $n_var; $i++ ) { 
  for ( $j = 0; $j < $nD; $j++ ) {
    $T['DeviceCrossVariant'][$device[$j]['name']][$i]['percentage'] = $_POST[$device[$j]["name"].'_'.$i];
  }
}
//-------------------------------------
// Call to Add/Edit Additional Variant Information
//print("<pre>".print_r($T,true)."</pre>");
$rslt =  set_device_specific_variant(json_encode($T)); //add_addnl_var_info(json_encode($X));
header("TestID: ".$id);
ob_clean();
?>

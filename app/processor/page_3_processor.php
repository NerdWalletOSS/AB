<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'db_get_test.php';
require_once 'set_device_specific_variant.php';
require_once "get_variant_idx.php";
require_once "db_get_rows.php";
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Set Device Specific Variant" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$test_id = $_POST['TestID'];
$T = db_get_test($test_id);
// SETTING NEW VALUES OF DEVICE X VARIANT
if(!isset($_POST['is_dev_specific']))  {
   $T['is_dev_specific'] = "0";
}
if (isset($_POST['is_dev_specific'])) {
  $T['is_dev_specific'] = $_POST['is_dev_specific'];

$nV = count($T['Variants']);
$this_device    = db_get_rows('device');
$nD = count($this_device);
for ( $i = 0; $i < $nV; $i++ ) { 
  for ( $j = 0; $j < $nD; $j++ ) {
    $T['DeviceCrossVariant'][$this_device[$j]['name']][$i]['percentage'] = $_POST[$this_device[$j]["name"].'_'.$i];
  }
}
}
//-------------------------------------
// Call to Set Device Specific Variant
$rslt =  set_device_specific_variant(json_encode($T));
print_r($rslt);
header("TestID: ".$rslt["TestID"]);
if (isset($rslt["msg_stderr"])) {
header("Error-Msg-Out: ".$rslt["msg_stderr"]);
}
//ob_clean();
?>

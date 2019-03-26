<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");

require_once 'db_get_test.php';
require_once 'set_filters.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

ob_start();
if ( !$_POST ) {
  echo '{ "Set Filters" : "ERROR", "Message" : "No paylaod" }'; exit;
}
$test_id = $_POST['TestID'];
$T = db_get_test($test_id);
// SETTING NEW VALUES OF DEVICE X VARIANT
if(!isset($_POST['has_filters']))  {
  $T['has_filters'] = "0";
}
if (isset($_POST['has_filters'])) {
  $T['has_filters'] = $_POST['has_filters'];
  if ( $T['has_filters'] == "1" )  {
    $nA = count($T['CategoricalFilters']);
    for ( $i = 0; $i < $nA; $i++ ) {
      if (isset($_POST['is_on_'.$i])) {
        $T['CategoricalFilters'][$i]['is_on'] = $_POST['is_on_'.$i];
      } else {
        $T['CategoricalFilters'][$i]['is_on'] = "0";
      }
    }
  }
}
//-------------------------------------
//print("<pre>".print_r($T,true)."</pre>");
// Call to Set Device Specific Variant
$rslt =  set_filters(json_encode($T));
header("TestID: ".$rslt["TestID"]);
ob_clean();
?>

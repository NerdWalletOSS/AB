<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
require_once "list_rts.php";
require_once "load_configs.php";
require_once "xurl.php";
function  chk_constants()
{
  if ( !load_configs() ) { 
    $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $SP = list_rts(); 
  if (!$SP ) { 
    $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
    return false; 
  }
  foreach ( $SP as $sp )  {
    $s = $sp['server']; $p = $sp['port'];
    // Check that servers are running 
    $rslt = xurl($s, $p, "Constants");
    if ( !$rslt ) { 
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    $X = json_decode($rslt);
    if ( !$X ) { 
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_num_devices'] != $X->{'MaxNumDevices'} ) { 
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_num_tests'] != $X->{'MaxNumTests'} ) { 
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['min_num_variants'] != $X->{'MinNumVariants'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    // TODO: Change > to == when UI gets better
    if ( $configs['max_num_variants'] > $X->{'MaxNumVariants'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_custom_data'] != $X->{'MaxLenCustomData'} ) {
      var_dump($X);
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_custom_data'] != $X->{'MaxLenCustomData'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_variant_name'] != $X->{'MaxLenVariantName'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_test_name'] != $X->{'MaxLenTestName'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_variant_dscr'] != $X->{'MaxLenVariantURL'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_cat_attr'] != $X->{'MaxLenCatAttr'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
    if ( $configs['max_len_cat_attr_val'] != $X->{'MaxLenCatAttrVal'} ) {
      $GLOBALS['err'] .=  "FILE: " . __FILE__ . " LINE: " . __LINE__ . " \n"; 
      return false; 
    }
  }
  return true;
}
/*
$GLOBALS['err'] = "";
$x = chk_constants();
if ( !$x ) { 
  echo "ERROR]n";
  var_dump($GLOBALS['err']);
}
else {
  echo "Constants match up\n";
}
 */
?>

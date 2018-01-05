<?php
function ssi_dev_spec_converter(
  $X /* input */
)
{
  // get number of devices 
  $num_devices = 0;
  for ( $i = 0; ; $i++, $num_devices++ ) { 
    $key = "device_" . $i; if ( !isset($X[$key]) ) { break; }
  }
  if ( $num_devices == 0 ) { return null; }
  // get number of URLs
  $num_urls = 0;
  for ( $i = 0; ; $i++, $num_urls++ ) { 
    $key = "desc_" . $i; if ( !isset($X[$key]) ) { break; }
  }
  if ( $num_urls == 0 ) { return null; }
  //--Accumulate urls in U
  $U = array($num_devices);
  for ( $i = 0; $i < $num_urls; $i++ ) { 
    $key = "desc_" . $i; 
    $U[$i] = $X[$key];
  }
  for ( $i = 0; $i < $num_devices; $i++ ) { 
    unset($Y);
    for ( $j = 0; $j < $num_urls; $j++ ) { 
      $key = "prop_" . $i . "_" . $j;
      if ( ( isset($X[$key]) ) && ( $X[$key] > 0 ) ) {
        $Y[$U[$j]] = $X[$key];
      }
    }
    if ( !$Y ) { return false; }
    $key = "device_" . $i; 
    $D[$X[$key]] = $Y;
  }
  return $D;
}
?>

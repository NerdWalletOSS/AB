<?php

// Execute URL 
function get_url( 
  $server,
  $port,
  $url,
  &$http_code,
  &$rslt,
  &$destination,
  $num_retries = 1
)
{
  $ch = curl_init();
  curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt ($ch, CURLOPT_FOLLOWLOCATION, true);
  curl_setopt($ch, CURLOPT_POST, 0);
  if ( ( is_null($port) ) || ( $port == "" ) ) { 
    $S = $server . "/";
  }
  else {
    $S = $server . ":" . $port . "/";
  }
  $full_url = $S . $url;
 //  echo "full_url = $full_url \n";
  curl_setopt($ch, CURLOPT_URL, $full_url);
  $tries = 0;
  for ( $tries = 0; $tries < $num_retries; $tries++ ) { 
    $rslt = curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $destination = curl_getinfo($ch, CURLINFO_EFFECTIVE_URL); // UTPAL: For detination URL
    if ( $http_code != 200 ) { 
      return false; 
    }
  } 
  curl_close($ch);
  return true;
}
/*
$http_code = 0;
$rslt = "";
$X = get_url("localhost", 8000, "TestInfo?TestName=Ramesh&TestType=XYTest", 200);
$X = get_url("localhost", 8020, "DescribeInstances", $http_code, $rslt);
    var_dump($rslt);
 */
?>

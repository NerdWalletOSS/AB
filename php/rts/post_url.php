<?php

// Execute URL 
function post_url( 
  $server,
  $port,
  $url,
  $body,
  &$http_code,
  &$rslt
)
{
  if ( !load_configs() ) { return null; }
  $configs = $GLOBALS['CONFIGS'];
  $ch = curl_init();
  curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt ($ch, CURLOPT_FOLLOWLOCATION, true);
  curl_setopt($ch, CURLOPT_POST, 0);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: text/plain')); 
  if ( ( is_null($port) ) || ( $port == "" ) ) { 
    $S = $server . "/";
  }
  else {
    $S = $server . ":" . $port . "/";
  }
  $full_url = $S . $url;
 //  echo "full_url = $full_url \n";
  curl_setopt($ch, CURLOPT_URL, $full_url);
  $num_retries = $configs['num_retries'];
  $tries = 0;
  for ( $tries = 0; $tries < $num_retries; $tries++ ) { 
    $rslt = curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ( $http_code == 200 ) { break; }
    trigger_error("RTS ERROR: $server, $url\n");
  } 
  curl_close($ch);
}
/*
$X = xurl("localhost", 8000, "TestInfo?TestName=Ramesh&TestType=XYTest", 200);
var_dump($X);
 */
?>

<?php
function chk_url(
  $url
)
{
  assert(isset($url));
  assert(is_string($url));
  $handle   = curl_init($url);
  assert($handle, "Unable to init curl\n");
  curl_setopt($handle, CURLOPT_TIMEOUT, 10);
  curl_setopt($handle, CURLOPT_HEADER, false);
  curl_setopt($handle, CURLOPT_FOLLOWLOCATION, true);
  curl_setopt($handle, CURLOPT_FAILONERROR, true);  // this works
  curl_setopt($handle, CURLOPT_RETURNTRANSFER, false);
  curl_setopt($handle, CURLOPT_NOBODY, true);
  $connectable = curl_exec($handle);
  curl_close($handle);
  return $connectable;
}
/*
$x = chk_url("djfsdjfhsdjf.yahoo.com");
$x = chk_url("www.yahoo.com");
var_dump($x);
 */
?>

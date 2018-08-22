<?php
require_once 'rs_assert.php';
function chk_url(
  $url
)
{
  rs_assert(isset($url));
  rs_assert(is_string($url));
  $handle   = curl_init($url);
  rs_assert($handle, "Unable to init curl");
  curl_setopt($handle, CURLOPT_TIMEOUT, 10);
  curl_setopt($handle, CURLOPT_HEADER, false);
  curl_setopt($handle, CURLOPT_FOLLOWLOCATION, true);
  curl_setopt($handle, CURLOPT_FAILONERROR, true);  // this works
  curl_setopt($handle, CURLOPT_RETURNTRANSFER, false);
  curl_setopt($handle, CURLOPT_NOBODY, true);
  $connectable = curl_exec($handle);
  curl_close($handle);
  if ( $connectable ) { return true; } else { return false; }
}
/*
$x = chk_url("djfsdjfhsdjf.xyahoo.com");
var_dump($x);
$x = chk_url("www.yahoo.com");
var_dump($x);
 */
?>

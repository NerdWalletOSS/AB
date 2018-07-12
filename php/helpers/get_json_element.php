<?php
require_once 'rs_assert.php';
function get_json_element(
  $J,
  $key,
  $die_if_missing=true
)
{
  rs_assert(isset($J));
  rs_assert(isset($key));
  if ( $die_if_missing ) {
    rs_assert(isset($J->{$key}));
  }
  if ( !isset($J->{$key}) ) { return null; }
  if ( is_string($J->{$key}) )  {
    return trim($J->{$key});
  }
  else {
    return $J->{$key};
  }
}
?>

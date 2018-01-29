<?php
require_once 'rs_assert.php';
function get_json_element(
  $J,
  $key
)
{
  rs_assert(isset($J));
  rs_assert(isset($key));
  rs_assert(isset($J->{$key}));
  if ( is_string($J->{$key}) )  {
    return trim($J->{$key});
  }
  else {
    return $J->{$key};
  }
}
?>

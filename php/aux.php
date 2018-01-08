<?php
function get_json(
  $J,
  $key
)
{
  assert(isset($J));
  assert(isset($key));
  assert(isset($J->{$key}));
  return $J->{$key};
}

function make_boolean(
  $in_str
)
{
  if ( !isset($in_str) || ( $in_str == "" ) || 
    ( strtolower($in_str) == "false" ) ) {
      return false;
    }
  else if ( strtolower($in_str) == "true" ) {
    return true;
  }
  else {
    return null;
  }
}

function make_pos_int(
  $in_str
)
{
  assert( ( isset($in_str) ) && ( $in_str != "" ) );
  $outval = intval($in_str);
  assert($outval > 0 );
  return $outval;
}
?>

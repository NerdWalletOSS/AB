<?php
function get_json(
  $J,
  $key
)
{
  assert(isset($J));
  assert(isset($key));
  assert(isset($J->{$key}));
  if ( is_string($J->{$key}) )  {
    return trim($J->{$key});
  }
  else {
    return $J->{$key};
  }
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
function is_unique(
  $X
)
{
  assert(isset($X));
  assert(is_array($X));
  assert(count($X) > 0 );
  for ( $i = 0; $i < count($X); $i++ ) { 
    for ( $j = $i+1; $j < count($X); $j++ ) { 
      assert($X[$i] != $X[$j]);
    }
  }
  return true;
}
function is_good_percs(
  $X
)
{
  assert(isset($X));
  assert(is_array($X));
  assert(count($X) > 0 );
  $sum = 0;
  for ( $i = 0; $i < count($X); $i++ ) { 
    assert(is_float($X[$i]));
    assert($X[$i] >= 0 );
    assert($X[$i] <= 100 );
    $sum += $X[$i];
  }
  assert($sum = 100);
  return true;
}
?>

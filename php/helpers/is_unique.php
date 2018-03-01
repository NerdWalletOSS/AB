<?php
require_once 'rs_assert.php';
// IMPORTANT: Equality comparison is case insensitive
function is_unique(
  $X
)
{
  rs_assert(isset($X));
  rs_assert(is_array($X));
  rs_assert(count($X) > 0 );
  for ( $i = 0; $i < count($X); $i++ ) { 
    for ( $j = $i+1; $j < count($X); $j++ ) { 
      rs_assert(strtolower($X[$i]) != strtolower($X[$j]), 
      "Uniqueness condition fails");
    }
  }
  return true;
}

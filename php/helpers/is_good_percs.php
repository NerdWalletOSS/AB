<?php
require_once 'rs_assert.php';
function is_good_percs(
  $X
)
{
  rs_assert(isset($X));
  rs_assert(is_array($X));
  rs_assert(count($X) > 0 );
  $nX = count($X);
  rs_assert($nX >= lkp('configs', "min_num_variants"));
  rs_assert($nX <= lkp('configs', "max_num_variants"));
  foreach ( $X as $p ) {
    rs_assert(is_float($p), "percentage must be a number");
    rs_assert($p >=   0, "percentage must not be negative");
    rs_assert($p <= 100, "percentage cannot exceed 100");
  }
  $sum = 0;
  for ( $i = 0; $i < count($X); $i++ ) { 
    rs_assert(is_float($X[$i]));
    rs_assert($X[$i] >= 0 );
    rs_assert($X[$i] <= 100 );
    $sum += $X[$i];
  }
  rs_assert($sum = 100);
  return true;
}

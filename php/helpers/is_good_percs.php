<?php
require_once 'rs_assert.php';
require_once 'lkp.php';
function is_good_percs(
  $X,
  $bin_type
)
{
  rs_assert(isset($X));
  rs_assert(is_array($X));
  rs_assert(count($X) > 0 );
  $nX = count($X);
  rs_assert($nX >= lkp('configs', "min_num_variants"));
  rs_assert($nX <= lkp('configs', "max_num_variants"));
  $sum = 0;
  foreach ( $X as $p ) {
    rs_assert(is_numeric($p), "percentage must be a number");
    rs_assert($p >=   0, "percentage must not be negative");
    rs_assert($p <= 100, "percentage cannot exceed 100");
    $sum += $p;
  }
  rs_assert($sum == 100);
  if ( $bin_type == "c_to_v_ok_v_to_c_ok_v_to_v_not_ok" ) { 
    // TODO

  }
  return true;
}

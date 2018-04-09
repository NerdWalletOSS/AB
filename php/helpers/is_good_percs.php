<?php
require_once 'rs_assert.php';
require_once 'lkp.php';
function is_good_percs(
  $P, // percentages
  $bin_type,
  $N // names
)
{
  rs_assert(isset($P));
  rs_assert(is_array($P));
  rs_assert(is_array($N));
  rs_assert(count($P) == count($N));
  rs_assert(count($P) > 0 );
  $nX = count($P);
  rs_assert($nX >= lkp('configs', "min_num_variants"), "too few variants specified");
  rs_assert($nX <= lkp('configs', "max_num_variants"),  "too many variants specified");
  $sum = 0;
  foreach ( $P as $p ) {
    rs_assert(is_numeric($p), "percentage must be a number");
    rs_assert($p >=   0, "percentage must not be negative");
    rs_assert($p <= 100, "percentage cannot exceed 100");
    $sum += $p;
  }
  rs_assert(( $sum < 100 + 0.0001 ) && ( $sum > 100 - 0.0001 ) );
  if ( $bin_type == "c_to_v_ok_v_to_c_ok_v_to_v_not_ok" ) { 
    $idx = 0;
    $nV = count($P);
    $max_perc_non_control = floor(100.0 / ($nV-1.0));
    foreach ( $P as $p ) {
      if ( $N[$idx] != "Control" )  {
        rs_assert($p <= $max_perc_non_control, 
          "max percentage for non-control variant is $max_perc_non_control");
      }
      $idx++;
    }
  }
  return true;
}

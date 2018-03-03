<?php
require_once 'is_unique.php';
require_once 'rs_assert.php';

function is_good_variants(
  $V,
  $bin_type
)
{
  rs_assert(isset($V));
  rs_assert(is_array($V));
  rs_assert(count($V) > 0 );
  $nV = count($V);
  rs_assert($nV >= lkp('configs', "min_num_variants"));
  rs_assert($nV <= lkp('configs', "max_num_variants"));
  rs_assert(is_unique($V), "variant names not unique");
  $is_control = false;
  foreach ( $V as $v ) {
    rs_assert(aux_chk_name($v), "variant name is invalid");
    rs_assert(strlen($v) <= lkp("configs", "max_len_variant_name"),
<<<<<<< HEAD
    "Variant name too long. Max length is " .  lkp("configs", "max_len_variant_name"));
=======
      "Variant name too long. Max length is " .  
      lkp("configs", "max_len_variant_name"));
>>>>>>> dev
    if ( $v == "Control" ) {
      $is_control = true;
    }
  }
  if ( $bin_type ==  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok" ) { 
    rs_assert($is_control, "Must have one variant called Control");
  }
  return true;
}
?>

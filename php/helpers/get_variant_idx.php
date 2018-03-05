<?php
function get_variant_idx(
  $T,
  $variant_id
)
{
  rs_assert(!empty($T));
  rs_assert(!empty($variant_id));
  rs_assert(is_numeric($variant_id));
  $Variants = $T['Variants']; // UTPAL: Fixed minor typing error, $T was written as $X.
  rs_assert($Variants);
  $found = false; 
  $vidx = 0;
  foreach ( $Variants as $v ) { 
    if ( $v['id'] == $variant_id ) { return $vidx; }
  $vidx++; // UTPAL: Added this line to increment the id, as earlier the functio was returning vidx as 0 always.
  }
  rs_assert(false, "Could not find variant ID $variant_id");
}
?>

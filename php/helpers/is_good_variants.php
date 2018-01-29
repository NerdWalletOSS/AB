<?php
require_once 'is_unique.php';
require_once 'rs_assert.php';

function is_good_variants(
  $V
)
{
  assert(isset($V));
  assert(is_array($V));
  assert(count($V) > 0 );
  $nV = count($V);
  assert($nV >= lkp('configs', "min_num_variants"));
  assert($nV <= lkp('configs', "max_num_variants"));
  assert(is_unique($V));
  foreach ( $V as $v ) {
    assert(aux_chk_name($v), "variant name is invalid");
    assert(strlen($v) <= lkp("configs", "max_len_variant_name"));
  }
  return true;
}
?>

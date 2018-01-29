<?php
require_once 'rs_assert.php';
require_once 'is_unique.php';
function is_good_urls(
  $U
)
{
  rs_assert(isset($U));
  rs_assert(is_array($U));
  rs_assert(count($U) > 0 );

  $nU = count($U);
  rs_assert($nU >= lkp('configs', "min_num_variants"));
  rs_assert($nU <= lkp('configs', "max_num_variants"));
  rs_assert(is_unique($U));
  foreach ( $U as $u ) {
    rs_assert(strlen($u) <= lkp("configs", "max_len_url"));
    rs_assert(chk_url_text($u), "Bad URL [$u]\n");
    $is_chk = lkp('configs', "check_url_reachable");
    if ( $is_chk ) { 
      rs_assert(chk_url($url), "URL [$u] not reachable\n");
    }
  }
  return true;
}
?>

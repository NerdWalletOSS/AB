<?php
require_once 'rs_assert.php';
require_once 'is_unique.php';
require_once 'chk_url.php';
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
  rs_assert(is_unique($U), "URLS need to be unique");
  foreach ( $U as $u ) {
    rs_assert(strlen($u) <= lkp("configs", "max_len_url"),
      "URL is too long. Max length is $max_len_url");
    rs_assert(chk_url_text($u), "Bad URL [$u]\n");
      "URL [$u] contains bad characters\n");
    $is_chk = lkp('configs', "check_url_reachable");
    if ( $is_chk ) { 
      rs_assert(chk_url($u), "URL [$u] not reachable\n");
    }
    else {
      // echo("Not checking URL [$u] for reachability");
    }
  }
  return true;
}
?>

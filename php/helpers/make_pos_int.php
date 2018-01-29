<?php
require_once 'rs_assert.php';
function make_pos_int(
  $in_str
)
{
  rs_assert( ( isset($in_str) ) && ( $in_str != "" ) );
  $outval = intval($in_str);
  rs_assert($outval > 0 );
  return $outval;
}

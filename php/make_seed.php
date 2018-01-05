<?php
function make_seed()
{
  $x = mt_rand(1, 2*1024*1048576);
  $y = mt_rand(1, 2*1024*1048576);
  $z = ($x << 32) + $y;
  return $z;
}
// print(make_seed());
?>

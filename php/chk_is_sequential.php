<?php
function chk_is_sequential(
    $variant_percs, 
    $nV
) 
{
  $non_zero_idx = -1;
  for ( $i = 0; $i < $nV; $i++ ) { // all but Control
    if ( $variant_percs[$i] > 0 ) { 
      if ( $non_zero_idx >= 0 ) { 
        $GLOBALS["err"] .= "Only one variant can be non-zero\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      if ( $variant_percs[$i] != 100 ) { 
        $GLOBALS["err"] .= "Non-zero variant must be 100 \n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
      $non_zero_idx = $i;
    }
  }
  return true;
}
?>

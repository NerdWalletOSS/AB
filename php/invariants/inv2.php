<?php
require_once 'dbconn.php';
require_once 'rs_assert.php';
function invariant_2(
)
{
  $dbh = dbconn();
  rs_assert($dbh);
  // Execute query. Say return value is $X

  foreach ( $X as $x ) {  
    $percentage  = $x['percentage'];
    $test_id     = $x['test_id'];
    $device_id   = $x['device_id'];
    rs_assert($pecentage == 100, 
      "Error: test $test_id, device_id $device_id, perc = $percentage\n");
    }
  }
  return true;
}
?>

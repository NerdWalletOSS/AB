<?php
require_once "bye.php";
require_once "dbconn.php";

function pr_state_history(
  $X,
  $admins,
  $states
)
{
  printf("modifier_id,modifier,state_id,state,when_changed\n");
  for ( $i = 0; $i  < count($X); $i++ ) { 
    $x = $X[$i];
    $when_changed = $x['when_changed'];
    $modifier_id  = $x['modifier_id'];
    $y            = $admins[$modifier_id];
    $modifier     = $y['name'];

    $state_id     = $x['state_id'];
    $y            = $states[$state_id];
    $state     = $y['name'];

    printf("$modifier_id,$modifier,$state_id,$state,$when_changed\n");
  }
}
?>

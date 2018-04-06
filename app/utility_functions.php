<?php
function bootstrap_state_action(
  $state_id, 
  &$color, 
  &$action, 
  &$action_name
) 
{
  switch ($state_id) {
  case "1":
    $color = "success";
    $action = "started";
    $action_name = "Start";
    break;
 case "2":
    $color = "danger";
    $action = "Terminate";
    $action_name = "Terminate";
    break;
 /*
  case "3":
    $color = "success";
    $action = "started";
    $action_name = "start";
    break;
*/
  case "4":
    $color = "primary";
    $action = "archived";
    $action_name = "Archive";
    break;
  case "5":
    $color = "info";
    $action = "no action";
    $action_name = "No Action";
    break;
  }
}


?>

<?php
require_once "db_get_test.php";
require_once "db_get_row.php";
require_once "lkp.php";

  // Decide Mode
{
  if ($id == "") { 
    $mode = "Add"; 
  }
  else {
    $rslt = db_get_test($id);
    if ($rslt == "") {
      header('Location: home.php'); return false; // TODO ERROR HANDLING
    }

    $id          = $rslt['id'];
    $TestName    = $rslt['name'];
    $description = $rslt['description'];
    $BinType     = $rslt['BinType'];
    $external_id = $rslt['external_id'];
    $state_id    = intval($rslt['state_id']);
    $this_state       = lkp("state", $state_id, "reverse");
    if ( $this_state == "started" ) {
      $can_fix_to_a_winner = true;
    }
    else {
      $can_fix_to_a_winner = false;
    }
    switch ( $this_state ) {
    case "draft" : 
    case "dormant" : 
    case "started" : 
      $mode = "Edit";
      break;
    case "terminated" : 
    case "archived" : 
      $mode = "View";
      break;
    default : 
      // HANDLE ERROR PROPERLY TODO
      break;
    }
  }
  // Decide Parameters on the basis of mode
  ## -- Mode = Add
  if ( $mode == "Add" ) {
    $readonly = "";
    $Ureadonly = "";
    $TestName = "";
    # -- Check if number of variants are set.
    if (!isset($_GET['num_var']) || ($_GET['num_var'] == "")) { header('Location: home.php'); return false;} 
    if (isset($_GET['num_var'])) {$num_var = $_GET['num_var'];} 
  }

  ## -- Mode = Edit
  if ( $mode == "Edit" )
  {
    $readonly = "";
    $Ureadonly = "";
    $Preadonly = "";
    if (isset($rslt['Channel'])) {$Channel = $rslt['Channel'];}
    if ($this_state == "draft") {$readonly = "";}
    if($TestType == "XYTest") { $num_var = count($rslt['Variants']); } else {$num_var = count($rslt['Variants']) - 1; }
  }

  ## -- Mode = View
  if ( $mode == "View" ) {
    $readonly = "readonly";
    $Ureadonly = "readonly";
    $Preadonly = "readonly";
    if (isset($rslt['Channel'])) {
      $Channel = $rslt['Channel'];
    }
    $num_var = count($rslt['Variants']) - 1;
    if ( $TestType == "XYTest") { 
      $num_var = count($rslt['Variants']); 
    } 
    else {
      $num_var = count($rslt['Variants']) - 1; 
    }
  }

  // LOGIC: Declaring Variables
  if ( isset($TestType) && ($TestType == 'XYTest')) { 
    $n_var = $num_var;
  }
  elseif ( isset($TestType) && ($TestType == 'ABTest')) { 
    $n_var = ($num_var+1);
  }
  //print("<pre>".print_r($rslt,true)."</pre>");
}
?>

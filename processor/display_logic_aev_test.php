<?php
require_once "db_get_test.php";
require_once "db_get_row.php";

// Decide Mode
if ($id == "") { $mode = "Add"; }
if ($id != "") {
  $rslt = db_get_test($id);
  if ($rslt == "") { header('Location: home.php'); return false; }
  else {
  $id = $rslt['id'];
  $TestName = $rslt['name'];
  $description = $rslt['description'];
  $BinType = $rslt['BinType'];
  $st = db_get_row("state", "id", $rslt['state_id']);
  $state = $st['name'];
  }
  if ($rslt['state_id'] != "2") { $can_fix_to_a_winner = "0"; }
  $state_id = $rslt['state_id'];
  if (($state_id == "1") || ($state_id == "2") || ($state_id == "3") ) { $mode = "Edit"; }
  if (($state_id == "4") || ($state_id == "5")) { $mode = "View"; }
}

// Decide Parameters on the basis of mode
## -- Mode = Add
if ( $mode == "Add" ) {
  $readonly = "";
  $TestName = "";
  # -- Check if number of variants are set.
  if (isset($_GET['num_var'])) {$num_var = $_GET['num_var'];} 
}

## -- Mode = Edit
if ( $mode == "Edit" )
{
	$readonly = "readonly";
	$Preadonly = "";
  if (isset($rslt['Channel'])) {$Channel = $rslt['Channel'];}
  if ($state == "draft") {$readonly = "";}
  if($TestType == "XYTest") { $num_var = count($rslt['Variants']); } else {$num_var = count($rslt['Variants']) - 1; }
}

## -- Mode = View
if ( $mode == "View" )
{
	$readonly = "readonly";
	$Preadonly = "readonly";
  if (isset($rslt['Channel'])) {$Channel = $rslt['Channel'];}
  $num_var = count($rslt['Variants']) - 1;
if($TestType == "XYTest") { $num_var = count($rslt['Variants']); } else {$num_var = count($rslt['Variants']) - 1; }
}

// LOGIC: Declaring Variables
if ( isset($TestType) && ($TestType == 'XYTest')) 
  { 
    $n_var = $num_var;
  }
elseif ( isset($TestType) && ($TestType == 'ABTest')) 
  { 
    $n_var = ($num_var+1);
  }
 //print("<pre>".print_r($rslt,true)."</pre>");
?>

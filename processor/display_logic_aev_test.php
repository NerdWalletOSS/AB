<?php
if ( $id == "") { 
  $mode = "Add"; 
  $readonly = "";
  $TestName = "";
# -- Check if number of variants are set.
if (isset($_GET['num_var'])) {$num_var = $_GET['num_var'];} 

} else {
  $mode = "Edit/View";
	$readonly = "readonly";
  $rslt = db_get_test($id);
  print("<pre>".print_r($rslt,true)."</pre>");
  $TestName = $rslt['name'];
  $num_var = count($rslt['Variants']) - 1;
}

// LOGIC: Declaring Variables
if ( isset($TestType) && ($TestType == 'XYTest')) 
  { $n_var = $num_var;}
elseif ( isset($TestType) && ($TestType == 'ABTest')) 
  { $n_var = ($num_var+1);}
?>

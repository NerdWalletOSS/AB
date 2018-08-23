<?php

// -- SET PATH
require_once "set_path.php";

// -- STANDARD HEADER INFORMATION
require_once "header.php";

// -- GET TESTS
require_once "dbconn.php";
require_once "db_get_rows.php";
require_once "utility_functions.php";
$state = "'3','4'";
if (isset($TestType))
	{
		if ($TestType == "ABTest")
			{
				$test_type_id = 1;
			}

		if ($TestType == "XYTest")
			{
				$test_type_id = 2;
			}

		$result = db_get_rows("test", "test_type_id = " . $test_type_id . " and state_id IN (" . $state . ")");
        }
require_once "html_header.php";
?>
<!-- PAGE SPECIFIC FILE CALLS -->
  <link href="css/dataTables.min.css" rel="stylesheet">
  <script src="js/dataTables.min.js"></script>
<!--
  <script src="js/filter_test.js"></script>
    <script src="js/check_test.js"></script>
-->
    <script src="js/home.js"></script>
</head>
<body>
<!-- STANDARD HEADER INFORMATION -->
<?php
require_once "navbar.php";
 ?>
<div class="container theme-showcase" role="main"> 
<div class="row">
<div class="col-xs-12">
<div class="panel panel-primary">
<div class="panel-heading">
  <h3 class="panel-title">Test Table &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Admin &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Select Admin to display the test cases created by them."></span>&nbsp;
  <select form="FilterAdmin" name='TestAdmin' style='color:white;background-color:grey;'>";
  <option value=''>All</option>
<?php 
$admin    = db_get_rows('admin');
$nA = count($admin);
for ( $i = 0; $i < $nA; $i++ ) { 
  echo "<option value='".$admin[$i]['id']."'"; 
  if((isset($User)) && ($User == $admin[$i]['name'])) {echo 'selected';}
  echo ">".$admin[$i]['name']."</option>";
} ?>
  </select>


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
	<input type="radio" name="option" value="'3','4'" id= "1" checked/>&nbsp;Started/Terminated  &nbsp;&nbsp;
  <input type="radio" name="option" value="2" id="2"  />&nbsp;Draft/Dormant &nbsp;&nbsp; 
  <input type="radio" name="option" value="3" id="3"  />&nbsp;Archive &nbsp;&nbsp; 
	<input type="hidden" name="TestType" id="TestType" value="
<?php
echo $TestType; 
?>
">
  <a href="#" data-toggle="modal" data-target="#basicModal">
    <button type="button" class="btn btn-success btn-xl">
      <span style="font-size:25px;"><strong>+</strong></span>
     </button>
  </a>
  </h3>
</div>
<div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
<?php
if (isset($TestType))
	{
		require_once "test_table.php";

	}
else
	{
		echo "<h1>Test Type not defined.</h1>";
	}
?>
         </div>
       </div>
     </div>
   </div>

 </div> 
<!-- /container -->
<?php
require_once "choose_num_variants.php";
 ?>
<!-- FOOTER -->
<?php
require_once "footer.php";
 ?>



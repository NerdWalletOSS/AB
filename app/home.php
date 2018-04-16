<?php

// -- SET PATH
require_once "set_path.php";

// -- STANDARD HEADER INFORMATION
require_once "header.php";

// -- GET TESTS
require_once "dbconn.php";
require_once "db_get_rows.php";
require_once "utility_functions.php";
$state = "'2','3','4'";
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
?>
<!-- PAGE SPECIFIC FILE CALLS -->
  <link href="css/dataTables.min.css" rel="stylesheet">
  <script src="js/dataTables.min.js"></script>
  <script src="js/filter_test.js"></script>
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
	<input type="radio" name="option" value="'2','3','4'" id= "1" checked/>&nbsp;Dormant/Started/Terminated  &nbsp;&nbsp;
  <input type="radio" name="option" value="2" id="2"  />&nbsp;Draft &nbsp;&nbsp; 
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



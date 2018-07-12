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
  <h3 class="panel-title">

  </h3>
</div>
<div class="panel-body">
<?php
if (isset($_GET['error'])) {
echo "<h2>".$_GET['error']."</h2>";
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



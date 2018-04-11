<?php 
// -- SET PATH
require_once "set_path.php";

// -- CALL REQUIRED FILES
require_once "header.php"; ?>
<script src="js/page_3.js"></script>
<?php 
require_once "navbar.php";
require_once "config_html.php";
require_once "db_get_rows.php";
require_once "db_get_test.php";
// -- TEST TestID CREDENTIALS
if (((!isset($_GET['TestID'])) || ($_GET['TestID'] == "")))
	{
		header('Location: home.php');
		return false;
	}
# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
$T = db_get_test($id);
require_once "display_logic_aev_test.php";
$config = config_html($TestType);
//print("<pre>".print_r($T,true)."</pre>");
?>

  <div class="container theme-showcase" role="main">
<?php 
if ($TestType == "ABTest" ) {
  require_once 'filters.php';
} else {
  require_once 'device_x_variant.php';
}
?>
  </div>


<!-- /container -->
<!-- FOOTER -->
<?php require_once "footer.php"; ?>

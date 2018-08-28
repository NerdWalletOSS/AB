<?php 
// -- SET PATH
require_once "set_path.php";

// -- CALL REQUIRED FILES
require_once "header.php";
require_once "db_get_rows.php";
require_once "db_get_test.php";
// -- TEST TestID CREDENTIALS
if (((!isset($_GET['TestID'])) || ($_GET['TestID'] == "")))
	{
		header('Location: error.php?error="TestID is not set"');
		return false;
	}
# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
$T = db_get_test($id);
require_once "display_logic_aev_test.php";
require_once "config_html.php";
$config = config_html($TestType);
require_once "html_header.php";
echo '<script src="js/page_3_'.$TestType.'.js"></script>';
require_once "navbar.php";
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

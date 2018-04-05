<?php require_once "common/header.php"; ?>
<script src="js/page_3.js"></script>
<?php require_once "common/navbar.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

require_once "processor/config_html.php";
require_once "db_get_rows.php";
require_once "db_get_test.php";
# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
$T = db_get_test($id);
require_once "processor/display_logic_aev_test.php";
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
<?php require_once "common/footer.php"; ?>

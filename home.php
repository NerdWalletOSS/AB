<?php
# -- CALL REQUIRED FILES 
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
# -- Get Active Tests

$is_archived = false;
if (isset($test_type)) {
$result = db_get_tests($test_type, $is_archived);
$nR = count($result);
}

?>
<?php require_once "common/header_1.php"; ?>
<?php require_once "common/header_2.php"; ?>
    <div class="container theme-showcase" role="main"> 
		<div class="row">
	        <div class="col-xs-12">
		  <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Active Test Table &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href="#" ><button type="button" style="align: right;" class="btn btn-sm btn-info">See Archived Test</button></a></h3>
            </div>
            <div class="panel-body">
<?php if (isset($TestType)) { require_once "panel/".$TestType."_active_test_table.php"; } 
       else { echo "<h1>Test Type not defined.</h1>";} 
?>
    </div></div></div></div>

    </div> <!-- /container -->

	<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

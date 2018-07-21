<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "includes/header_admin.php";
?>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Check Tests</h3>
            </div>
            <div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "../includes/error_div.php"; ?>
  <!-- AJAX ERROR DIV END --><h5>Click the button to check tests and match RTS information of tests vs database</h5>
							<form id="CheckTests"><input class="btn btn-lg btn-warning btn-block" type="submit" form='CheckTests' id="check_tests" value="Check Tests"></form>
            </div></div>
          </div>
        </div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

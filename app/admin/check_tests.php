<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "includes/header_admin.php"; 
require_once "chk_tests.php";
?>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Check Tests</h3>
            </div>
            <div class="panel-body">
<?php
$rslt = chk_tests();
print("<pre>".print_r($rslt,true)."</pre>");

?>
            </div></div>
          </div>
        </div></div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

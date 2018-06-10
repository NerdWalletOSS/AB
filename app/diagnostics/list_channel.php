<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "includes/header_diagnostics.php"; 
require_once "db_get_rows.php";
?>
<script src="js/add_channel.js"></script>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">LIST OF CHANNEL
</h3>

            </div>
            <div class="panel-body">
			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>S.No.</b></td><td><b>CHANNEL NAME</b></td></tr>
<?php
$admin = db_get_rows('channel');
$nR = count($admin); for ( $i = 0; $i < $nR; $i++ ) {
	echo "<tr><td>".$admin[$i]['id']."</td><td>".$admin[$i]['name']."</td></tr>";
    }
?>
</table>
            </div></div>
          </div>
        </div></div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

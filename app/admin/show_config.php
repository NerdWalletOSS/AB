<?php session_start();
require_once "../includes//header_admin.php"; 
require_once "../../php/load_configs.php";
?>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">CONFIGURATION TABLE</h3>
            </div>
            <div class="panel-body">
			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>KEY</b></td><td><b>VALUE</b></td></tr>
<?php
if ( !load_configs() ) { echo "ERROR"; }
$c = $GLOBALS['CONFIGS'];
  foreach ($c as $k => $v) {
        if ( $k == 'dbpass') { // Do Nothing 
        } else {
	echo "<tr><td>".$k."</td><td>".$v."</td></tr>";
        }
    }
?>
</table>
            </div></div>
          </div>
        </div></div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

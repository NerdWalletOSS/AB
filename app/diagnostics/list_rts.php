<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php");

require_once "../../php/rts/list_rts.php";
require_once "../includes/header_diagnostics.php"; 
?>

<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">LIST OF RUN TIME SERVER &nbsp; &nbsp;

</h3>

            </div>
            <div class="panel-body">

			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>SERVER</b></td><td><b>PORT</b></td></tr>
<?php
$SP = list_rts();
if ( $SP ) { 
   foreach ( $SP as $sp ) { 
       $server = $sp['server']; $port = $sp['port'];
	echo "<tr><td>".$server."</td><td>".$port."</td></tr>";
    }
}
?>
</table>
            </div></div>
          </div>
        </div></div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

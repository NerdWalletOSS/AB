<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "get_url.php";
require_once "includes/header_diagnostics.php"; 
$UserAgent = $_GET['UserAgent'];
$url = 'ClassifyUA?UserAgent='.urlencode($UserAgent);
$http_code = 0;
$rslt = "";
$data = get_url( 'localhost', '8000',$url, $http_code, $rslt, $destination );
//-----------------------------------------------------------
?>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title"><strong>USER AGENT:</strong> <?php echo $UserAgent; ?>
</h3>

            </div>
            <div class="panel-body">
<?php

if (!$data) {
echo "<strong>User Agent unavailable</strong>";
} else {
?>
			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>KEY</b></td><td><b>VALUE</b></td></tr>
<?php 
$result = json_decode($rslt);
  foreach ($result as $k => $v) {
	echo "<tr><td>".$k."</td><td>".$v."</td></tr>";
    }
?>
</table>
            </div>
<?php
}
?>
          </div>
        </div></div>
</div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

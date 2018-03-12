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
require_once "find_tests_to_follow.php";
# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
$T = db_get_test($id);
require_once "processor/display_logic_aev_test.php";
$config = config_html($TestType);
$fo_tests =  find_tests_to_follow($Channel);
var_dump($GLOBALS['state']); exit;
$nF = count($fo_tests);

//print("<pre>".print_r($T,true)."</pre>");
?>

  <div class="container theme-showcase" role="main">
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?> Follow On</h3>
  </div>
  <div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "common/error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
  <!-- ADD/EDIT FORM START  -->
  <form class="form-signin" id='device_x_variant' type='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td>Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
    <input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Channel: <?php echo $Channel; ?> <input type='hidden' name='Channel' value='<?php echo $Channel; ?>'>
  </td>
	</tr>
<tr>
<table class="table table-striped table-condensed" style="space=5px">
<thead>
<th>TestID</th>
<th>TestName</th>
<th>&nbsp;</th>
</thead>
<tbody>
<?php 
for ( $fidx = 0; $i < $nF; $fidx++ ) { 
  echo "<tr>";
  echo "<td>".$fo_tests[$fidx]['id']."<input type='hidden' name='TID_to_follow' value='".$fo_tests[$fidx]['id']."'></td>";
  echo "<td>".$fo_tests[$fidx]['name']."<input type='hidden' name='TNAME_to_follow' value='".$fo_tests[$fidx]['name']."'></td>";
  echo "</tr>";
  } ?>
<tr>
<td colspan="3"><button class="btn btn-lg btn-success btn-block" type="submit" id="follow_on">Next</button></td>
</form>
<td></td>
<td colspan="3">  <a href="thankyou.php?TestID=<?php echo $id; ?>"><button class="btn btn-lg btn-warning btn-block" >Skip</button></a></td>

8934988782
</tr>
</tbody>
</table>
</td>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
 </table>

</div>
</div>
</div>
</div>
</div>


<!-- /container -->
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

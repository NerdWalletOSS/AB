<?php require_once "common/header_1.php"; ?>
<script src="js/page_3.js"></script>
<?php require_once "common/header_2.php"; ?>
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
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?> Device X Variant</h3>
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
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Is Device Specific: <input type="checkbox" name="is_dev_specific" value="1" >
  </td>
	</tr>
<tr>
<table class="table table-striped table-condensed" style="space=5px">
<thead>
<th>Variants\Devices</th>
<?php 
$device    = db_get_rows('device');
$nD = count($device);
for ( $i = 0; $i < $nD; $i++ ) { 
  echo "<th>".$device[$i]['name']."</th>";
} ?>
</thead>
<tbody>
<?php for ( $i = 0; $i < $n_var; $i++ ) { 
  echo "<tr>";
  echo "<td>".$T['Variants'][$i]['name']."</td>";
    for ( $j = 0; $j < $nD; $j++ ) {
      echo "<td><input type='text' maxlength='3' size='3' name='".$device[$j]['name']."_".$i."' value='".$T['DeviceCrossVariant'][$device[$i]['name']][$i]['percentage']."'></td>";
        }
  echo "</tr>";

  } ?>
<tr>
<td colspan="3"><button class="btn btn-lg btn-success btn-block" type="submit" id="device_x_variant">Next</button></td>
<td></td>
<td colspan="3">  <a href="aev_test_4.php?TestID=<?php echo $id; ?>"><button class="btn btn-lg btn-warning btn-block" >Skip</button></a></td>

</tr>
</tbody>
</table>
</td>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
 </table>
</form>
</div>
</div>
</div>
</div>
</div>


<!-- /container -->
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?> Setting Filters</h3>
  </div>
  <div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
  <!-- ADD/EDIT FORM START  -->
  <form class="form-signin" id='setting_filters' method='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td colspan="2">Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'></td>
		<td colspan="3">Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
    <input type='hidden' name='TestType' value='<?php echo $TestType; ?>'></td>
	</tr>
<tr>
<td>
<tr>
<td>Attribute</td>
<td>Value</td>
<td>Checkbox</td>
</tr>
<tr>
<td>Platform</td>
<td>Desktop</td>
<td><input type="checkbox" name="attribute" value="Desktop"></td>
</tr>
<tr>
<td>Platform</td>
<td>Tablet</td>
<td><input type="checkbox" name="attribute" value="Tablet"></td>
</tr>
<tr>
<td>Platform</td>
<td>Mobile</td>
<td><input type="checkbox" name="attribute" value="Mobile"></td>
</tr>
<tr>
<td></td>
<td></td>
<td></td>
<tr>
<tr>
<td>Is Paid</td>
<td>true</td>
<td><input type="checkbox" name="is_paid" value="true"></td>
</tr>
<tr>
<td>Is Paid</td>
<td>False</td>
<td><input type="checkbox" name="is_paid" value="false"></td>
</tr>

<tr>
<td colspan="2"> <button onclick="location.href = 'aev_test_2.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-primary btn-block" >Previous</button></td>
<?php 
if ( $mode == "View" ) 
  {
    echo "<td colspan='3'>&nbsp;</td>";
  }
else
  { 
?>
<td colspan="3"><button class="btn btn-lg btn-success btn-block" type="submit" id="dev_x_var">Next</button></td>
<?php } ?>
<td colspan="2"> <button onclick="location.href = 'aev_test_4.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-warning btn-block" >Skip</button></td>
</tr>
</tbody>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
 </table>
</form>
</div>
</div>
</div>
</div>

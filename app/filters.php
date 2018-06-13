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
  <form class="form-signin" id='set_filters' method='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td>Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'></td>
		<td>Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
    <input type='hidden' name='TestType' value='<?php echo $TestType; ?>'></td>
 <td colspan="2">Has Filters? :&nbsp;&nbsp; 
<?php if ( $mode == "View" ) 
  { ?>
<?php if (isset($T['has_filters']) && ( $T['has_filters'] == "1")) { echo "Set True"; } else { echo "Not Set"; } ?>
<?php } else { ?>
<input type="checkbox" name="has_filters" value="1"  
<?php if (isset($T['has_filters']) && ( $T['has_filters'] == "1")) { echo "checked"; } else { // Do Nothing
} 
?>
>
<?php } ?>
</td>
	</tr>
<tr>
<td>
<tr>
<td>Attribute</td>
<td>description</td>
<td>Set</td>
</tr>
<?php 
$cat_attr_val = db_get_rows('cat_attr_val');
$cat_count = count($cat_attr_val);
for ($i = 0; $i < $cat_count; $i++) {
if (($cat_attr_val[$i]['name'] == "true") || ($cat_attr_val[$i]['name'] == "false")) { $is_paid = "1";} else { $is_paid = "0";}
?>
<tr>
<td><?php  if ($is_paid == "1"){echo "<strong>IsPaid</strong>";} else {echo "Platform"; }?></td>
<td><?php echo $cat_attr_val[$i]['name'];?></td>
<td>
<?php if ( $mode == "View" ) 
  { ?>
<?php if (isset($T['CategoricalFilters'][$i]['is_on']) && ( $T['CategoricalFilters'][$i]['is_on'] == "1")) { echo "Set True"; } else { echo "Not Set"; } ?>
<?php } else { ?>
<input type="checkbox" name="is_on_<?php echo $i; ?>" value="1"  
<?php if (isset($T['CategoricalFilters'][$i]['is_on']) && ( $T['CategoricalFilters'][$i]['is_on'] == "1")) { echo "checked "; } else { // Do Nothing
} 
?>
>
<?php } ?>

</td>
</tr>
<?php } ?>
<tr>
<td></td>
<td></td>
<td></td>
</tr>
<input type="hidden" name="cat_count" value="<?php echo $cat_count;?>">
<tr>
<td> <button onclick="location.href = 'aev_test_2.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-primary btn-block" >Previous</button></td>
<?php 
if ( $mode == "View" ) 
  {
    echo "<td>&nbsp;</td>";
  }
else
  { 
?>
<td><input class="btn btn-lg btn-success btn-block" type="submit" form="set_filters" id="setFilters" value="Save"></td>
<?php } ?>
<td> <button onclick="location.href = 'aev_test_4.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-warning btn-block" >Skip</button></td>
</tr>
</tbody>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
 </table>
</form>
</div>
</div>
</div>
</div>

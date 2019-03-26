  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?> Device X Variant</h3>
  </div>
  <div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
  <!-- ADD/EDIT FORM START  -->

  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
  <form class="form-signin" id='device_x_variant' method='POST'>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td colspan="1">Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'></td>
		<td colspan="2">Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
    <input type='hidden' name='TestType' value='<?php echo $TestType; ?>'></td>
    <td colspan="2">Is Device Specific: 
<?php if (( $mode == "View" ) || ($this_state == "started"))  
  { ?>
<?php if (isset($T['is_dev_specific']) && ( $T['is_dev_specific'] == "1")) { echo "<b style='color:blue'>True</b>"; echo "<input type='hidden' name='is_dev_specific' value='1'>";} else { echo "<b style='color:red'>Not Set</b>"; echo "<input type='hidden' name='is_dev_specific' value='0'>"; } ?>
<?php } else { ?>
<input type="checkbox" name="is_dev_specific" value="1"  id="is_dev_specific"  <?php if (isset($T['is_dev_specific']) && ( $T['is_dev_specific'] == "1")) { echo "checked";} ?>>
<?php } ?>
  </td>
<?php if (isset($T['is_dev_specific']) && ( $T['is_dev_specific'] == "1")) { ?>
<td colspan="2">Show Variant Stats? : &nbsp; &nbsp;<input type="checkbox" name="is_dcv_stats" value="1"  id="is_dcv_stats" data-id="<?php echo $id; ?>" ></td>
<?php } else { echo "<td colspan='2'></td>"; } ?>
	</tr>
<tr>
<td>
<tr>
<td>Variants\Devices</td>
<?php 
$all_device    = db_get_rows('device');
$nD = count($all_device);
for ( $i = 0; $i < $nD; $i++ ) { 
  echo "<td>".$all_device[$i]['name']."</td>";
} ?>
</tr>
<?php for ( $i = 0; $i < $n_var; $i++ ) { 
  echo "<tr>";
  echo "<td>".$T['Variants'][$i]['name']."</td>";
    for ( $j = 0; $j < $nD; $j++ ) {
      if($mode == "View") {
        echo "<td>".$T['DeviceCrossVariant'][$all_device[$j]['name']][$i]['percentage']."</td>";
      }
      elseif (($mode == "Edit")) {
        echo "<td><input type='text' class='dev_specific_variant' maxlength='3' size='3' name='".$all_device[$j]['name']."_".$i."' value='".$T['DeviceCrossVariant'][$all_device[$j]['name']][$i]['percentage']."'"; if (!isset($T['is_dev_specific']) || ( $T['is_dev_specific'] != "1")) { echo "readonly"; } echo "></td>";
       } 
        else { // Do Nothing
       }
        }
  echo "</tr>";

  } ?>
</form>
<tr>
<td colspan="2"> <button onclick="window.location.href = 'aev_test_2.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-primary btn-block" >Previous</button></td>
<?php 
if ( $mode == "View" ) 
  {
    echo "<td colspan='3'>&nbsp;</td>";
  }
else
  { 
?>
<td colspan="3"><input class="btn btn-lg btn-success btn-block" type="submit" form="device_x_variant" id="dev_x_var" value="Save"></td>
<?php } ?>
<td colspan="2"> <button onclick="window.location.href = 'aev_test_4.php?TestID=<?php echo $id; ?>';" class="btn btn-lg btn-warning btn-block" >Skip</button></td>
</tr>
</tbody>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
 </table>

</div>
</div>
</div>
</div>

<!-- VARIANT STATS TABLE -- DATA WILL BE POPULATED ON CHECKING THE SHOW VARIANT STATS CHECKBOX -->
<div id="dcv_stats" class="show hidden"></div>

<script>
$("#is_dev_specific").change(function (e) {
    e.preventDefault();
    var ischecked= $(this).is(':checked');
    if(ischecked) {
    $('.dev_specific_variant').removeAttr('readonly');
} else {
    $('.dev_specific_variant').attr('readonly', true);
}
})
</script>

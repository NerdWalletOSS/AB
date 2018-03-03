<?php require_once "common/header_1.php"; ?>
<script src="js/add_addln_var_info.js"></script>
<?php require_once "common/header_2.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

require_once "processor/config_html.php";

# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
require_once "processor/display_logic_aev_test.php";
$config = config_html($TestType);
?>

  <div class="container theme-showcase" role="main">
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?> Additional Variant Information</h3>
  </div>
  <div class="panel-body">
  <!-- AJAX ERROR DIV START -->
  <?php require_once "common/error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
  <!-- ADD/EDIT FORM START  -->
  <form class="form-signin" id='addTest' type='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td>Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
  </td>
	</tr>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
  <?php
  if ( isset($TestType) && ($TestType == "XYTest")) {
  for ( $i = 0; $i < $num_var; $i++ ) { 
  ?>
  <tr> 
   <td>
   <p>Variant ID: &nbsp; <span id='VID_<?php echo $i; ?>' > <?php echo $rslt['Variants'][$i]['id'];  ?></span>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    Variant Name: &nbsp;<span id='VName_<?php echo $i; ?>'><?php echo $rslt['Variants'][$i]['name']; ?></span>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="#myModal" class="btn btn-success btn-small" id="custId" data-toggle="modal" data-id="<?php echo $rslt['Variants'][$i]['id']; ?>"
    data-position="<?php echo $i; ?>">Edit</a></p>
    <p><strong>Description:</strong> &nbsp;<span id="Vdesc_<?php echo $i; ?>"><?php echo $rslt['Variants'][$i]["description"]; ?></span></p>
    <p><strong>Custom Data:</strong> &nbsp;<span id="Vcd_<?php echo $i; ?>"><?php echo $rslt['Variants'][$i]['custom_data']; ?></span></p>
  </td>
</tr>
<?php } } elseif ($TestType == "ABTest") { ?>
  <tr>
<input type='hidden' name='VID_0' value='<?php if ($mode != "Add") {echo $rslt['Variants'][0]['id']; } ?>'>
  <td>Original Feature&nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Standard Feature is called Control"></span>
  <input type="text" name="VName_0" size="16" maxlength="15" value="control" readonly="readonly"></td>
  <td>Description
  <textarea class="form-control" rows="3" cols="9" maxlength="128"  readonly="readonly" >Standard Feature</textarea></td>
  <td></td>
  <td>Percentage &nbsp;&nbsp;
  <input type="number"  style='width:5em' min="0" max="100" maxlength="3" size='2' id="control"  class="control" name="VPercentage_0" value="<?php echo $rslt['Variants'][0]['percentage']; ?>" readonly="readonly"></td>
  </tr>
<?php 
  for ( $i = 1; $i <= $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
?>
  <tr>
   <input type='hidden' name='<?php echo "VID_".$i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
  <td>Variant <?php echo $i + 1; ?>&nbsp;&nbsp;( ID : <?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;)&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' readonly required></td>
  <td>Description &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Human-readable description for what this variant is, i.e. `the blue apply now button link.`'></span>
  <textarea class='form-control' rows='3' cols='9' maxlength='128' name='var".$i."_desc' readonly required></textarea>
</td>
<td></td>
  <td>Percentage &nbsp;&nbsp;<input type='number' style='width:5em' min='0' max='<?php echo (100/$num_var); ?>' step='1' size='2' name='VPercentage_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>" class='prop' readonly required></td>
<td><a href="#" data-toggle="modal" data-target="#basicModal" ><button type="button" class="btn btn-success btn-sm">
<span style="font-size:15px;"><strong>Edit</strong></span>
</button>
</a></td>
<?php } ?>
  </tr>
<?php } ?>
<tr><td>
<a href="aev_test_3.php?TestID=<?php echo $id; ?>"><button class="btn btn-lg btn-warning btn-block" >Skip</button></a>
</td></tr>
  </tbody>
  </table>
  
  </form>
  <!-- ADD/EDIT FORM END  -->
  </div>
  </div>
  </div>
  </div>
  </div>

<div class="modal fade" id="myModal" role="dialog">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">&times;</button>
                <h4 class="modal-title">Additional Variant Information</h4>
            </div>
            <div class="modal-body">
                <div class="fetched-data">
<form id='addVI' type='post'>
<input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
<input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<input type='hidden' name='VariantID'>
<input type='hidden' name='VariantName'>
<table>
  <tr>
<td>
<p>Variant ID: &nbsp; <span id='VariantID' ></span></p>
<p>Variant Name: &nbsp;<span id='VariantName'></span></p>
<p><strong>Description:</strong> &nbsp;<textarea class='form-control' rows='3' cols='100' maxlength='256' name='Description' required></textarea></p>
<p><strong>Custom Data:</strong> &nbsp;<textarea class='form-control' rows='8' cols='100' maxlength='2048' name='CustomData' required></textarea></p>
</td>
</tr>
</table>
<button class="btn btn-lg btn-success btn-block" type="submit" id="addln_vi_2">Submit</button>
</form>
						</div> 
            </div>
            <div class="modal-footer">
								
            </div>
        </div>
    </div>
</div>

<!-- /container -->
<script>
//prop calculator
$('.prop').keyup(function () {
  var sum = 0;
  $('.prop').each(function() {
  sum += Number($(this).val());
<?php if ( isset($TestType) && ($TestType == 'ABTest')) { ?>
  control=100-sum;
  $('#control').val(control);
  $('#TotalProp').val(sum+control);
<?php } elseif ( isset($TestType) && ($TestType == 'XYTest')){ ?>
  $('#TotalProp').val(sum);
<?php  } ?>
});
});
</script>
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

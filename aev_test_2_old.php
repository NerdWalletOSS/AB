<?php require_once "common/header_1.php"; ?>
<script src="js/insert_test.js"></script>
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
  <?php if(($mode == "Edit") || ($mode == "View")) { ?>
	<tr>
		<td colspan="4">Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'></td>
	</tr>
	<tr>
		<td colspan="4">Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'></td>
	</tr>
 <tr> 
  <td colspan="4">Description &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#"></span>
  <textarea class="form-control" rows="3" cols="20" name="TestDescription"><?php echo $description;?></textarea>
  <input type='hidden' name='TestDescription' value='<?php echo $description; ?>'>
  </td>	
  </tr>
  <?php } else { /* Do Nothing value="<?php echo $TestName; ?>" <?php if ($id != "") {echo "readonly"; } ?> */ } ?>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
  <?php
  if ( isset($TestType) && ($TestType == "XYTest")) {
  for ( $i = 0; $i < $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
  ?>
  <tr> 
   <input type='hidden' name='VID_<?php echo $i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
    <td>Variant <?php echo $i + 1; ?>&nbsp;:<?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" readonly required></td>
  <td>Landing Page URL &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Absolute URL of the landing page for this variant.'></span>
  <input type='url' name='VURL_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['url'];} ?>" readonly required>
  </td>
  <td>Distribution:&nbsp;&nbsp; 
  <input type='text' style='width:5em'  size='3' name='VPercentage_<?php echo $i; ?>' class='prop' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>" readonly required></td></tr>
<?php } }elseif ($TestType == "ABTest") { ?>
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


  <tr>
    <td></td>
    <td >TOTAL: &nbsp; &nbsp;
     <input type="number" id="TotalProp" max="100"  size='3' value='<?php if(($mode == "Edit") || ($mode == "View")) {echo "100"; } ?>' disabled>
     <span style="color: Red">(Total must be 100*)</span>
    </td>
    <td></td>
  </tr>
  </tbody>
  </table>
  <button class="btn btn-lg btn-success btn-block" type="submit" id="add_test_2">Next</button>
  </form>
  <!-- ADD/EDIT FORM END  -->
  </div>
  </div>
  </div>
  </div>
  </div>


<div class="modal fade" id="basicModal" tabindex="-1" role="dialog" aria-labelledby="basicModal" aria-hidden="true">
  <div class="modal-dialog modal-lg">
  <div class="modal-content">
  <div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title" id="myModalLabel">Edit Description:</h4>
    </div>
    <form class="test" name="test" action="aev_test_1.php">
    <div class="modal-body">
    <div class="form-group">


  <tr>
   <input type='hidden' name='<?php echo "VID_".$i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
  <td>Variant&nbsp;&nbsp;( ID : <?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;)&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' readonly required></td>
  <td>Description &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Human-readable description for what this variant is, i.e. `the blue apply now button link.`'></span>
  <textarea class='form-control' rows='3' cols='9' maxlength='128' name='var".$i."_desc' readonly required></textarea>
</td>
<td></td>
  <td>Percentage &nbsp;&nbsp;<input type='number' style='width:5em' min='0' max='<?php echo (100/$num_var); ?>' step='1' size='2' name='VPercentage_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>" class='prop' readonly required></td>
  </tr>


<input type='hidden' name='NumVariants' value="<?php echo $n_var; ?>">
<?php if ($mode == "Add") { ?>
<input type='hidden' name='Creator' value='<?php echo $User; ?>'>
<?php } else { ?>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<input type='hidden' name='State' value='<?php echo $state; ?>'>
<?php } ?>
<input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>

  <input type='hidden' name='TestID' value=''>
       </div>
     </div>
      <div class="modal-footer">
      <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      <input class="btn btn-success" type="submit" value="Add Test" id="submit">
      </div>
      </form>
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

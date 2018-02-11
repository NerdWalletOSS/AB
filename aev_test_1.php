<?php require_once "common/header_1.php"; ?>
<script src="js/insert_test.js"></script>
<?php require_once "common/header_2.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

require_once "processor/config_html.php";
require_once "db_get_test.php";

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
  <h3 class="panel-title"><?php echo $mode; ?> Test</h3>
  </div>
  <div class="panel-body">
  <?php require_once "common/error_div.php"; ?>
  <form class="form-signin" id='addTest' type='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
  <tr>
  <td colspan="3">Test Name &nbsp; 
    <span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a descriptive name for your test that represents your vertical, date, and test. It should be easy to read and memorable. .Only Alphanumeric char without space">
    </span>
  <input type="text" name="TestName" size="<?php echo $config['name_size']; ?>" maxlength="<?php echo $config['name_maxlength']; ?>" pattern="^[A-Za-z0-9\S]{1,31}$" value="<?php echo $TestName; ?>" <?php echo $readonly; ?> required>
  </td>
  </tr>
  <tr>
  <td colspan="3">Description &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a friendly description for what the test is for and what it is trying to validate. Please include a wiki link. "></span>
  <textarea class="form-control" rows="3" cols="20" name="TestDescription" 
    maxlength="<?php echo $config['desc_maxlength']; ?>" 
    <?php echo $readonly; ?> required>
<?php if (isset($description)) {echo $description;}?>
  </textarea>
  </td>	
  </tr>
<?php
if ( isset($TestType) && ($TestType == "XYTest")) {
  for ( $i = 0; $i < $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
?>
  <tr> 
   <input type='hidden' name='VID_<?php echo $i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
    <td>Variant <?php echo $i; ?>&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' required></td>
  <td>Landing Page URL &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Absolute URL of the landing page for this variant.'></span>
  <input type='url' name='VURL_<?php echo $i; ?>' required>
  </td>
  <td>Distribution:&nbsp;&nbsp; 
  <input type='text' style='width:5em'  size='3' name='VPercentage_<?php echo $i; ?>' class='prop' required></td></tr>
<?php } }else { ?>
  <tr>
  <td>Original Feature&nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Standard Feature is called Control"></span>
  <input type="text" name="VName_0" size="16" maxlength="15" value="control" readonly="readonly"></td>
  <!--<td>Description
  <textarea class="form-control" rows="3" cols="9" maxlength="128"  readonly="readonly" >Standard Feature</textarea></td>-->
  <td></td>
  <td>Percentage &nbsp;&nbsp;
  <input type="number"  style='width:5em' min="0" max="100" maxlength="3" size='2' id="control"  class="control" name="VPercentage_0" value="<?php echo $rslt['Variants'][0]['percentage']; ?>" readonly="readonly"></td>
  </tr>
<?php 
  for ( $i = 1; $i <= $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
?>
  <tr>
   <input type='hidden' name='<?php echo "VID_".$id; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
  <td>Variant <?php echo $i; ?>&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' required></td>
  <!--<td>Description &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Human-readable description for what this variant is, i.e. `the blue apply now button link.`'></span>-->
<!--
  <textarea class='form-control' rows='3' cols='9' maxlength='128' name='var".$i."_desc' required></textarea>
-->
</td>
<td></td>
  <td>Percentage &nbsp;&nbsp;<input type='number' style='width:5em' min='0' max='<?php echo (100/$num_var); ?>' step='1' size='2' name='VPercentage_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>" class='prop' required></td>
<?php } ?>
  </tr>
<?php } ?>

<input type='hidden' name='NumVariants' value="<?php echo $n_var; ?>">
<input type='hidden' name='Creator' value='<?php echo $User; ?>'>
<input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
<input type='hidden' name='TestID' value='<?php echo $id; ?>'>
  <tr>
    <td></td>
    <td >TOTAL: <br/>
      <span style="color: Red">Total must be 100*</span>
      <input type="text" id="TotalProp" max="100" disabled>
    </td>
    <td></td>
  </tr>
  </tbody>
  </table>
  <button class="btn btn-lg btn-success btn-block" type="submit" id="add_test">Next</button>
  </form>
  </div>
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

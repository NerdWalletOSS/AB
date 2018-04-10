<?php 
// -- SET PATH
require_once "set_path.php";

// -- TEST TestID CREDENTIALS
if (((!isset($_GET['TestID'])) || ($_GET['TestID'] == "")))
	{
		header('Location: home.php');
		return false;
	}
# -- CHECK IF TEST ID IS SET
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}

// -- STANDARD HEADER INFORMATION
require_once "header.php"; 
?>
<script src="js/insert_test.js"></script>
<?php 
require_once "navbar.php";
require_once "display_logic_aev_test.php";
require_once "config_html.php";

# -- CONFIGURE HTML CONSTRAINTS
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
  <!-- AJAX ERROR DIV START -->
  <?php require_once "error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
  <!-- ADD/EDIT FORM START  -->

  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
  <form class="form-signin" id='addTest' method='POST'>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
  <?php if(($mode == "Edit") || ($mode == "View")) { ?>
	<tr>
		<td colspan="3">Test ID: <?php echo $id; ?></td>
	</tr>
	<tr>
		<td colspan="3">Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'></td>
	</tr>
  <?php } elseif (($mode == "Add")) { ?>
  <tr>
  <td colspan="3">Test Name &nbsp; 
    <span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a descriptive name for your test that represents your vertical, date, and test. It should be easy to read and memorable. .Only Alphanumeric char without space">
    </span>
  <input type="text" name="TestName" size="<?php echo $config['name_size']; ?>" maxlength="<?php echo $config['name_maxlength']; ?>" pattern="^[A-Za-z0-9\S]{1,31}$" required>

  </td>
  </tr>
  <td>

  </td>
  </tr>
  <?php } else { /* Do Nothing value="<?php echo $TestName; ?>" <?php if ($id != "") {echo "readonly"; } ?> */ } ?>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->

  <tr> 
  <td colspan="3">Description &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a friendly description for what the test is for and what it is trying to validate. Please include a wiki link. "></span>
  <textarea class="form-control" rows="3" cols="20" name="TestDescription" 
    maxlength="<?php echo $config['desc_maxlength']; ?>" 
    <?php echo $readonly; ?> required>
  <?php if (isset($description)) {echo $description;}?>
  </textarea>
  </td>	
  </tr>
<tr><td>
  <?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>

Channel &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Select to which channel does this experiment belongs to?"></span>&nbsp;
<?php 
if ( $mode == "View" ) 
  {
    if (isset($Channel)) { echo $Channel;}
  }
else
  { 
?>
  <select name='Channel'>";
  <option value=''>None</option>
<?php 
$channel    = db_get_rows('channel');
$nC = count($channel);
for ( $i = 0; $i < $nC; $i++ ) { 
  echo "<option value='".$channel[$i]['name']."'"; 
  if((isset($Channel)) && ($Channel == $channel[$i]['name'])) {echo 'selected';}
  echo ">".$channel[$i]['name']."</option>";
} ?>
  </select>
<?php } ?>
</td></tr>	
<?php
  for ( $i = 0; $i < $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
  ?>
  <tr> 
   <input type='hidden' name='VID_<?php echo $i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
    <td>Variant <?php echo $i + 1; ?>&nbsp;:<?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_<?php echo $i; ?>' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" <?php echo $readonly; ?> required></td>
  <td>Landing Page URL &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Absolute URL of the landing page for this variant.'></span>
  <input type='text' name='VURL_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['url'];} ?>" <?php echo $Ureadonly; ?> required>
  </td>
  <td>Distribution:&nbsp;&nbsp; 
  <input type='text' style='width:5em'  size='3' name='VPercentage_<?php echo $i; ?>' class='prop' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>"required <?php echo $readonly; ?>></td></tr>
<?php } }elseif ($TestType == "ABTest") { ?>
  <tr>
<input type='hidden' name='VID_0' value='<?php if ($mode != "Add") {echo $rslt['Variants'][0]['id']; } ?>'>
  <td>Original Feature&nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Standard Feature is called Control"></span>
  <input type="text" name="VName_0" size="16" maxlength="15" value="Control" readonly="readonly"></td>
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
   <input type='hidden' name='<?php echo "VID_".$i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
  <td>Variant <?php echo $i + 1; ?>&nbsp;&nbsp;( ID : <?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;)&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
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
<input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input type='hidden' name='NumVariants' value="<?php echo $n_var; ?>">
<?php if ($mode == "Add") { ?>
<input type='hidden' name='Creator' value='<?php echo $User; ?>'>
<?php } else { ?>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<input type='hidden' name='State' value='<?php echo $this_state; ?>'>
<?php } ?>
<input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
<input type='hidden' name='BinType' value='<?php if (isset($BinType)) { echo $BinType;} ?>'>
  <tr>
    <td></td>
    <td >TOTAL: &nbsp; &nbsp;
     <input type="number" id="TotalProp" max="100"  size='3' value='<?php if(($mode == "Edit") || ($mode == "View")) {echo "100"; } ?>' disabled>
     <span style="color: Red">(Total must be 100*)</span>
    </td>
    <td> </td>
  </tr>
<tr>
<td><button onclick="window.location.href = 'home.php'"  class="btn btn-lg btn-primary btn-block" >Cancel</button></td>
<?php 
if ( $mode == "View" ) 
  {
    echo "<td>&nbsp;</td>";
  }
else
  { 
?>
<td><input class="btn btn-lg btn-success btn-block" type="submit" id="add_test" value="Next"></td>

<?php } ?>
  <?php if(($mode == "Edit") || ($mode == "View")) { ?>
<td> <button onclick="window.location.href = 'aev_test_2.php?TestID=<?php echo $id; ?>';"  class="btn btn-lg btn-warning btn-block" >Skip</button></td>
<?php } ?>
</tr>
  </form>
  </tbody>
  </table>



  <!-- ADD/EDIT FORM END  -->
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
<?php require_once "footer.php"; ?>

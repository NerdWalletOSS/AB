<?php 
// -- SET PATH
require_once "set_path.php";
// -- TEST TestID CREDENTIALS
if (!isset($_GET['TestID'])) {
		header('Location: error.php?error="TestID is not set"');
		return false;
	} else {
  $id = $_GET['TestID'];
}

# -- Get Tests
if (isset($id) && ($id != "")) {
  require_once "db_get_test.php";
  $T = db_get_test($id);
}
// -- STANDARD HEADER INFORMATION
require_once "header.php"; 
require_once "display_logic_aev_test.php";
require_once "html_header.php";
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
<div class="alert hidden" id="modalAlert"></div>
  <!-- ADD/EDIT FORM START  -->

  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
<form  id='TestURL' class="form-signin" method="POST"></form>
  <form class="form-signin" id='addTest' method='POST'></form>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
  <?php if(($mode == "Edit") || ($mode == "View")) { ?>
	<tr>
		<td colspan="3">Test ID: <?php echo $id; ?></td>
	</tr>
	<tr>
		<td colspan="3">Test Name: <?php echo $TestName; ?>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Clone this test:</b> &nbsp;&nbsp;
<a href='#'  class='OpenCloneModal'  data-id="<?php echo $id; ?>" data-name="<?php echo $TestName; ?>" data-creator="<?php echo $User; ?>">
    <button type='button' class='btn btn-warning btn-sm'>
      <span style='font-size:15px;'><strong>+</strong></span>
     </button>
      <input form="addTest" type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
  </td>
	</tr>
  <?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>
  <tr>
  <td colspan="3">Test URL: &nbsp;&nbsp;<a href='http://www.nerdwallet.com/ur2?nw_campaign_id=<?php echo $external_id; ?>' >
  http://www.nerdwallet.com/ur2?nw_campaign_id=<?php echo $external_id; ?></a></td>
  </tr>
  <tr>
  <td>Device :<?php if (isset($T['is_dev_specific']) && ( $T['is_dev_specific'] == "1")) { echo "<b style='color:blue'>The test is DEVICE SPECIFIC</b>&nbsp;&nbsp;". "<input form='TestURL' type='text' name='device' ></td><td ><input class='btn btn-sm btn-primary btn-block' type='submit' form='TestURL' id='test_url' value='Test URL'>". "<input form='TestURL' type='hidden' name='nw_campaign_id' value=".$external_id.">"; } 
else { 
echo "<b style='color:red'>The test is NOT Device Specific</b>"; 
echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Show Variant Stats? : &nbsp; &nbsp;<input type='checkbox' name='is_vc_stats' value='1'  id='is_vc_stats' data-id='".$id."' ></td>";
} 
?> 



</td><td>&nbsp;</td>
  </tr>
  <?php 
          } 
   ?>
  <?php } elseif (($mode == "Add")) { ?>
  <tr>
  <td colspan="3">Test Name &nbsp; 
    <span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a descriptive name for your test that represents your vertical, date, and test. It should be easy to read and memorable. .Only Alphanumeric char without space">
    </span>
  <input form="addTest" type="text" name="TestName" size="<?php echo $config['name_size']; ?>" maxlength="<?php echo $config['name_maxlength']; ?>" pattern="^[A-Za-z0-9\S]{1,31}$" required>

  </td>
  </tr>
  <td>

  </td>
  </tr>
  <?php } else { /* Do Nothing value="<?php echo $TestName; ?>" <?php if ($id != "") {echo "readonly"; } ?> */ } ?>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
  <tr> 
  <td colspan="3">Description &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a friendly description for what the test is for and what it is trying to validate. Please include a wiki link. "></span>
  <textarea form="addTest" class="form-control" rows="3" cols="20" name="TestDescription" 
    maxlength="255" 
    <?php echo $readonly; ?> required>
  <?php if (isset($description)) {echo $description;}?>
  </textarea>
  </td>	
  </tr>
<tr><td >
  <?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>

Channel &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Select to which channel does this experiment belongs to?"></span>&nbsp;
<?php 
if (($mode != "Add") && (( $this_state == "started" ) || ( $this_state == "terminated" ) || ( $this_state == "archived" ) ))
  {
    if (isset($Channel)) { echo $Channel;}
  }
else 
  { 
?>
  <select form="addTest" name='Channel'>";
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
</td>
<td colspan="2">
<?php
if ( isset($TestType) && ($TestType == "XYTest")) {
  if ( $mode == "Edit" ) {
    if ($this_state == "started") { ?>
Change URL(s)?:&nbsp;&nbsp;<input form="addTest" type="checkbox" data-toggle="modal" data-target="#ConfirmationModal" name="OverWriteURL" value="true" >
<?php 
    } 
  } 
} 
?>
</td>
</tr>	
<?php
  for ( $i = 0; $i < $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
  ?>
  <tr> 
   <input form="addTest" type='hidden' name='VID_<?php echo $i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
    <td>Variant <?php echo $i + 1; ?>&nbsp;:<?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input form="addTest" type='text' size='16' name='VName_<?php echo $i; ?>' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" <?php echo $readonly;   if ( $mode == "Edit" ) { if ($this_state == "started") { echo "readonly";} } ?> required></td>
  <td>Landing Page URL &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Absolute URL of the landing page for this variant.'></span>
  <input form="addTest" type='url' class='btn btn-default'  name='VURL_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['url'];} ?>" id="url_<?php echo $i; ?>" 
<?php 
if (($mode == "Edit") && ($this_state == "started")) { 
  echo "readonly"; 
} else {
  echo $Ureadonly; 
} 
?> required>
  </td>
  <td>Distribution:&nbsp;&nbsp; 
  <input form="addTest" type='text' style='width:5em'  size='3' name='VPercentage_<?php echo $i; ?>' class='prop' 
value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>"required <?php echo $readonly; ?> <?php if (isset($T['is_dev_specific']) && ( $T['is_dev_specific'] == "1")) { echo "readonly"; }?>></td></tr>
<?php } }elseif ($TestType == "ABTest") { ?>
  <tr>
<input form="addTest"  type='hidden' name='VID_0' value='<?php if ($mode != "Add") {echo $rslt['Variants'][0]['id']; } ?>'>
  <td>Original Feature&nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Standard Feature is called Control"></span>
  <input form="addTest" type="text" name="VName_0" size="16" maxlength="15" value="Control" readonly="readonly"></td>
  <td></td>
  <td>Percentage &nbsp;&nbsp;
  <input form="addTest" type="number"  style='width:5em' min="0" max="100" maxlength="3" size='2' id="control"  class="control" name="VPercentage_0" value="<?php echo $rslt['Variants'][0]['percentage']; ?>" readonly="readonly"></td>
  </tr>
<?php 
  for ( $i = 1; $i <= $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
?>
  <tr>
   <input form="addTest" type='hidden' name='<?php echo "VID_".$i; ?>' value='<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['id']; } ?>'>
  <td>Variant <?php echo $i + 1; ?>&nbsp;&nbsp;( ID : <?php if ($mode == "Edit") {echo $rslt['Variants'][$i]['id'];} ?>&nbsp;)&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input form="addTest" type='text' size='16' name='VName_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['name']; } ?>" maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' required></td>

</td>
<td></td>
  <td>Percentage &nbsp;&nbsp;<input form="addTest" type='number' style='width:5em' min='0' max='<?php echo (100/$num_var); ?>' step='1' size='2' name='VPercentage_<?php echo $i; ?>' value="<?php if ($mode != "Add") {echo $rslt['Variants'][$i]['percentage'];} ?>" class='prop' required></td>
<?php } ?>
  </tr>
<?php } ?>
<input form="addTest" type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input form="addTest" type='hidden' name='NumVariants' value="<?php if (isset($n_var)) {echo $n_var; }?>">
<?php if ($mode == "Add") { ?>
<input form="addTest" type='hidden' name='Creator' value='<?php echo $User; ?>'>
<?php } else { ?>
<input form="addTest"type='hidden' name='Updater' value='<?php echo $User; ?>'>
<input form="addTest" type='hidden' name='State' value='<?php echo $this_state; ?>'>
<?php } ?>
<input form="addTest" type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
<input form="addTest" type='hidden' name='BinType' value='<?php if (isset($BinType)) { echo $BinType;} ?>'>
  <tr>
    <td></td>
    <td >TOTAL: &nbsp; &nbsp;
     <input type="number" id="TotalProp" max="100"  size='3' value='<?php if(($mode == "Edit") || ($mode == "View")) {echo "100"; } ?>' disabled>
     <span style="color: Red">(Total must be 100*)</span>
    </td>
    <td> </td>
  </tr>
<tr>
<td><a href="home.php"><button class="btn btn-lg btn-primary btn-block" >Cancel</button></a></td>
<?php 
if ( $mode == "View" ) 
  {
    echo "<td>&nbsp;</td>";
  }
else
  { 
?>
<td><input class="btn btn-lg btn-success btn-block" type="submit" form="addTest" id="add_test" value="Save"></td>

<?php } ?>
  <?php if(($mode == "Edit") || ($mode == "View")) { ?>
<td> <button onclick="window.location.href = 'aev_test_2.php?TestID=<?php echo $id; ?>';"  class="btn btn-lg btn-warning btn-block" >Skip</button></td>
<?php } ?>
</tr>
  </tbody>
  </table>

<?php require_once "confirmation_modal.php"; ?>
<?php require_once "prompt_clone_a_test.php"; ?>
  <!-- ADD/EDIT FORM END  -->
  </div>
  </div>
  </div>
  </div>

<!-- VARIANT STATS TABLE -- DATA WILL BE POPULATED ON CHECKING THE SHOW VARIANT STATS CHECKBOX -->
<div id="vc_stats" class="show hidden"></div>
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
<script>
$("#btn_ok").click(function (e) {
  $('#ConfirmationModal').modal('hide');
  // Get the value of NumVariants
  var NumVar = $("input[name=NumVariants]" ).val();
  var i = 0;
  while (i < NumVar) {
    $('#url_'+i).removeAttr('readonly');
    i++;
  }
})
</script>

<!-- FOOTER -->
<?php require_once "footer.php"; ?>

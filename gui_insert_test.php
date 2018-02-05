<?php require_once "common/header_1.php"; ?>
<script src="js/insert_test.js"></script>
<?php require_once "common/header_2.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

# -- Check if number of variants are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
if ( $id == "") { $mode = "Add"; } else {$mode = "Edit/View";}
if ($id != "") {
require_once "db_get_test.php";
$rslt = db_get_test($id);
//var_dump($rslt);
}
$num_var = "";
$GLOBALS["err"] = "";
unset($_SESSION["err"]);
if (isset($_GET['num_var'])) {$num_var = $_GET['num_var'];} else {
  $GLOBALS["err"] .= "ERROR: Number of Vraiants not set.\n";
  $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  $_SESSION["err"] = nl2br($GLOBALS["err"]);
  header('Location: home.php');	
}
?>
  <div class="container theme-showcase" role="main">
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"><?php echo $mode; ?>   Test</h3>
  </div>
  <div class="panel-body">
<?php require_once "common/error_div.php"; ?>
  <form class="form-signin" id='addTest' type='post'>
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
  <tr>
  <td colspan="3">Test Name &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a descriptive name for your test that represents your vertical, date, and test. It should be easy to read and memorable. .Only Alphanumeric char without space"></span>
  <input type="text" name="TestName"  
<?php if ( isset($TestType) && ($TestType == "XYTest")) {
  echo 'size="127" maxlength="127"';
  } else {
  echo 'size="63"  maxlength="63"';
  }
?> pattern="^[A-Za-z0-9\S]{1,31}$" required <?php if ($mode != "Add") { echo "readonly='readonly'"; echo "value=".$rslt['name']."";

} ?>

>
  </td>

  </tr>
  <tr>
  <td style="width: 33%">Description &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Provide a friendly description for what the test is for and what it is trying to validate. Please include a wiki link. "></span>
  <textarea class="form-control" rows="3" cols="9" name="TestDescription" maxlength="128" required></textarea>
  </td>
  <td>
<?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>
Channel &nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Select to which channel does this experiment belongs to?"></span>
  <select name='Channel'>";
  <option value=''>None</option>
<?php
foreach ($channel as $value) {
  echo "<option value='".$value['name']."'>".$value['name']."</option>";
}
?>
  </select>
<?php } ?>	
  </td>	
  </tr>
<?php
if ( isset($TestType) && ($TestType == "XYTest")) {
  for ( $i = 0; $i < $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
  echo "<tr>"; 
  echo "<td>Variant ".$i."&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='variant".$i."' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' required></td>
  <td>Landing Page URL &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Absolute URL of the landing page for this variant.'></span>
  <input type='url' name='var".$i."_desc' required>
  </td>
  <td>Distribution:&nbsp;&nbsp; 
  <input type='text' style='width:5em'  size='3' name='prop_".$i."' class='prop' required></td></tr>";
} 
} else { ?>
  <tr>
  <td>Original Feature&nbsp;<span class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="tooltip" href="#" data-original-title="Standard Feature is called Control"></span>
  <input type="text" name="VName_0" size="16" maxlength="15" value="control" readonly="readonly"></td>
  <td>Description
  <textarea class="form-control" rows="3" cols="9" maxlength="128"  readonly="readonly" >Standard Feature</textarea></td>
  <td>Percentage &nbsp;&nbsp;
  <input type="number"  style='width:5em' min="0" max="100" maxlength="3" size='2' id="control"  class="control" name="VPercentage_0" readonly="readonly"></td>
  </tr>
<?php 
  for ( $i = 1; $i <= $num_var; $i++ ) { 
  $max_prop = (100 /($num_var));
  echo "<tr>";
  echo "<td>Variant ".$i."&nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Code-readable name for this variant as used by engineering. Should be descriptive with no spaces or special characters, i.e. apply_now_blue. Only Alphanumeric char without space'></span>
  <input type='text' size='16' name='VName_".$i."' maxlength='15' pattern='^[A-Za-z0-9\S]{1,15}$' required></td>
  <td>Description &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title=' Human-readable description for what this variant is, i.e. `the blue apply now button link.`'></span>
<!--
  <textarea class='form-control' rows='3' cols='9' maxlength='128' name='var".$i."_desc' required></textarea>
-->
</td>
  <td>Percentage &nbsp;&nbsp;<input type='number' style='width:5em' min='0' max='".(100/$num_var)."' step='1' size='2' name='VPercentage_".$i."' class='prop' required></td>";
}
  echo "</tr>";
}
?>
<input type='hidden' name='NumVariants' value="<?php if ( isset($TestType) && ($TestType == 'XYTest')) 
{ echo $num_var;} 
elseif ( isset($TestType) && ($TestType == 'ABTest')) 
{ print($num_var+1);} 
?>">
<?php echo "
  <input type='hidden' name='Creator' value='".$User."'>
  <input type='hidden' name='TestType' value='".$TestType."'>"; ?>
  <input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>
<tr><td></td><td></td><td >TOTAL: <br/><span style="color: Red">Total must be 100*</span><input type="text" id="TotalProp" max="100" disabled></td><td></td></tr>
<?php } ?>
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
$('.prop').keyup(function () {
  var sum = 0;
  $('.prop').each(function() {
  sum += Number($(this).val());
<?php if ($TestType== "ABTest"){ ?>
  control=100-sum; 
<?php } ?>
  total=sum;
 });
<?php if ( isset($TestType) && ($TestType == 'ABTest')){ ?>
  $('#control').val(control);
<?php } ?>
  $('#TotalProp').val(sum);
});
</script>
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

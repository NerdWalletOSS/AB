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
?>
  <div class="container theme-showcase" role="main">
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"> Add Filters</h3>
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
<td>
<table style="border: none; border-collapse: collapse;">
<tbody>
<tr style="height: 21pt;">
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Attribute </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Value </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Checkbox </span></p>
</td>
</tr>
<tr style="height: 21pt;">
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Platform </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Desktop </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">&nbsp;</td>
</tr>
<tr style="height: 21pt;">
<td style="vertical-align: middle; border: solid #000000 1pt;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Platform </span></p>
</td>
<td style="vertical-align: middle; border: solid #000000 1pt;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Tablet </span></p>
</td>
<td style="vertical-align: middle; border: solid #000000 1pt;">&nbsp;</td>
</tr>
<tr style="height: 21pt;">
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Platform </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">Mobile </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">&nbsp;</td>
</tr>
<tr style="height: 13pt;">
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">IsPaid </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">true </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 1pt; border-top: solid #000000 3pt; vertical-align: middle;">&nbsp;</td>
</tr>
<tr style="height: 13pt;">
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">IsPaid </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">
<p style="line-height: 1.2; margin-top: 0pt; margin-bottom: 12pt;"><span style="font-size: 13pt; font-family: Times; color: #000000; background-color: transparent; font-weight: 400; font-variant: normal; text-decoration: none; vertical-align: baseline; white-space: pre-wrap;">false </span></p>
</td>
<td style="border-left: solid #000000 1pt; border-right: solid #000000 1pt; border-bottom: solid #000000 3pt; border-top: solid #000000 1pt; vertical-align: middle;">&nbsp;</td>
</tr>
</tbody>
</table>
</td>
</tr>

<?php echo "
  <input type='hidden' name='Creator' value='".$User."'>
  <input type='hidden' name='TestType' value='".$TestType."'>"; ?>
  <input type='hidden' name='TestID' value='<?php echo $id; ?>'>
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

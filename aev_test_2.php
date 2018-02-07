<?php require_once "common/header_1.php"; ?>
<script src="js/insert_test.js"></script>
<?php require_once "common/header_2.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

# -- Check if number of variants are set.
if (isset($_GET['id'])) {$id = $_GET['id'];}
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
<table border="1" width="100%">
<thead>
<th>Attribute</th>
<th>Value</th>
<th>Checkbox</th>
</thead>
<tr>
<td>Platform</td>
<td>Desktop</td>
<td><input type="checkbox" name="filter" value="platform:desktop"></td>
</tr>
<tr>
<td>Platform</td>
<td>Tablet</td>
<td><input type="checkbox" name="filter" value="platform:tablet"></td>
</tr>
<tr>
<td>Platform</td>
<td>Mobile</td>
<td><input type="checkbox" name="filter" value="platform:mobile"></td>
</tr>
<td>Is Paid</td>
<td>true</td>
<td><input type="checkbox" name="filter" value="paid:true"></td>
</tr>
<td>Is Paid</td>
<td>false</td>
<td><input type="checkbox" name="filter" value="paid:false"></td>
</tr>
</thead>
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

<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

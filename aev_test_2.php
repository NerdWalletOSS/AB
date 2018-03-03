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
  <?php for ( $i = 0; $i < $n_var; $i++ ) { ?>
  <tr> 
   <td>
   <p>Variant ID: &nbsp; <span id='VID_<?php echo $i; ?>' > <?php echo $rslt['Variants'][$i]['id'];  ?></span>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    Variant Name: &nbsp;<span id='VName_<?php echo $i; ?>'><?php echo $rslt['Variants'][$i]['name']; ?></span>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<?php   if ( ($rslt['Variants'][$i]['name'] == "Control") && ($TestType == "ABTest")) { 
// Do Nothing
} else {
?>
   <a href="#myModal" class="btn btn-success btn-small" id="custId" data-toggle="modal" data-id="<?php echo $rslt['Variants'][$i]['id']; ?>"
    data-position="<?php echo $i; ?>">Edit</a></p>
<?php } ?>
    <p><strong>Description:</strong> &nbsp;<span id="Vdesc_<?php echo $i; ?>"><?php echo $rslt['Variants'][$i]["description"]; ?></span></p>

<?php   if ( isset($TestType) && ($TestType == "XYTest")) { ?>
    <p><strong>Custom Data:</strong> &nbsp;<span id="Vcd_<?php echo $i; ?>"><?php echo $rslt['Variants'][$i]['custom_data']; ?></span></p>
<?php } ?>
  </td>
</tr>
<?php } ?>
 
<tr><td>
</td></tr>
  </tbody>
  </table>
  
  </form>
<a href="aev_test_3.php?TestID=<?php echo $id; ?>"><button class="btn btn-lg btn-warning btn-block" >Skip</button></a>
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
  <!-- AJAX ERROR DIV START -->
  <?php require_once "common/error_div.php"; ?>
  <!-- AJAX ERROR DIV END -->
                <div class="fetched-data">
<form id='addVI' type='post'>
<input type='hidden' name='TestType' value='<?php echo $TestType; ?>'>
<input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<input type='hidden' name='VariantID'>
<input type='hidden' name='VariantName'>
<input type='hidden' name='Position'>
<table>
  <tr>
<td>
<p>Variant ID: &nbsp; <span id='VariantID' ></span></p>
<p>Variant Name: &nbsp;<span id='VariantName'></span></p>
<p><strong>Description:</strong> &nbsp;<textarea class='form-control' rows='3' cols='100' maxlength='256' name='Description' required></textarea></p>
<?php   if ( isset($TestType) && ($TestType == "XYTest")) { ?>
<p><strong>Custom Data:</strong> &nbsp;<textarea class='form-control' rows='8' cols='100' maxlength='2048' name='CustomData' required></textarea></p>
<?php } ?>
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
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

<?php require_once "common/header.php"; ?>
<script src="js/fix_to_a_winner.js"></script>
<?php require_once "common/navbar.php"; ?>
<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");

require_once "processor/config_html.php";

# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
require_once "processor/display_logic_aev_test.php";
if (isset($can_fix_to_a_winner )  && ( $can_fix_to_a_winner == "0")) { header('Location: home.php'); return false; }
$config = config_html($TestType);
?>

  <div class="container theme-showcase" role="main">
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title"> Fix to a winner</h3>
  </div>
  <div class="panel-body">

  <!-- ADD/EDIT FORM START  -->
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>

  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME START -->
	<tr>
		<td colspan="2">Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
  </td>
	</tr>
  <!-- DISPLAY LOGIC FOR TEST ID & TEST NAME END -->
  <form class="form-signin" id='fix_to_a_winner' method='post'>
  <?php for ( $i = 0; $i < $n_var; $i++ ) { ?>
  <tr> 
   <td>
   <p>Variant ID: &nbsp; <?php echo $rslt['Variants'][$i]['id'];  ?>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    Variant Name: &nbsp;<?php echo $rslt['Variants'][$i]['name']; ?>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <p><strong>Description:</strong><?php echo $rslt['Variants'][$i]["description"]; ?></p>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<?php if ( isset($TestType) && ($TestType == "XYTest")) { ?>
    <p><strong>URL:</strong> &nbsp;<?php echo $rslt['Variants'][$i]['url']; ?></p>
<?php } ?>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  </td>
<td><input type="radio" name="Winner" value="<?php echo $rslt['Variants'][$i]['name']; ?>"></td>
</tr>
<?php } ?>
 <input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<tr>
<td><button class="btn btn-lg btn-danger btn-block"  type="submit" id="fix_to_a_winner">Fix to a winner</button></td>
  </form>
<td>  <a href="home.php"><button class="btn btn-lg btn-warning btn-block" >Cancel</button></a></td>
</tr>
  </tbody>

  </table>


  <!-- ADD/EDIT FORM END  -->
  </div>
  </div>
  </div>
  </div>
  </div>


<!-- /container -->
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

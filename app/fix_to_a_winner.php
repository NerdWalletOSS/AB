<?php 

// -- SET PATH
require_once "set_path.php";
require_once "header.php"; ?>
<script src="js/fix_to_a_winner.js"></script>
<?php 
require_once "navbar.php"; 
require_once "config_html.php";

// -- TEST TestID CREDENTIALS
if (((!isset($_GET['TestID'])) || ($_GET['TestID'] == "")))
	{
		header('Location: home.php');
		return false;
	}

# -- Check if number of TestID are set.
if (isset($_GET['TestID'])) {$id = $_GET['TestID'];}
require_once "display_logic_aev_test.php";
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
		<td colspan="2">
    Test ID: <?php echo $id; ?><input type='hidden' name='TestID' value='<?php echo $id; ?>'>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    Test Name: <?php echo $TestName; ?><input type='hidden' name='TestName' value='<?php echo $TestName; ?>'>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <?php echo "
    <a href='processor/set_state_processor.php?TestID=".$id."&state_id=4&action=resurrect'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Resurrect</button></a>
    "; ?>
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
<td><input type="radio" name="options" id="option1" autocomplete="off" name="Winner" value="<?php echo $rslt['Variants'][$i]['name']; ?>"></td>
</tr>
<?php } ?>
 <input type='hidden' name='TestID' value='<?php echo $id; ?>'>
<input type='hidden' name='Updater' value='<?php echo $User; ?>'>
<tr>
<td><button class="btn btn-lg btn-danger btn-block" data-toggle="confirmation" data-title="Are you sure?"  type="submit" id="fix_to_a_winner">Fix to a winner</button></td>
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
<script src="js/bootstrap-confirmation.js"></script>
<!-- FOOTER -->
<?php require_once "footer.php"; ?>

<?php require_once "common/header_1.php"; ?>
<?php
# -- CALL REQUIRED FILES 
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");
# -- Get Active Tests
require_once "db_get_rows.php";
$state = '2, 3 and 4';
if (isset($TestType)) {
if ($TestType == "ABTest") { $test_type_id = 1; }
if ($TestType == "XYTest") { $test_type_id = 2; }
$result = db_get_rows("test", "test_type_id = ".$test_type_id." and state_id = '".$state."'");
//$nR = count($result);
}
?>
<link href="css/dataTables.min.css" rel="stylesheet">
<script src="js/dataTables.min.js"></script>
<script src="js/filter_test.js"></script>

<script>

</script>
<?php require_once "common/header_2.php"; ?>
    <div class="container theme-showcase" role="main"> 
		<div class="row">
	        <div class="col-xs-12">
		  <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">Test Table &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
								<input type="radio" name="option" value="1" />Default  &nbsp;&nbsp;
                <input type="radio" name="option" value="2" />Draft &nbsp;&nbsp; 
                <input type="radio" name="option" value="3" />Archive &nbsp;&nbsp; 
								<input type="hidden" name="TestType" id="TestType" value="<?php echo $TestType; ?>">
<!--<button type="button" style="align: right;" class="btn btn-sm btn-info">See Archived Test</button></a>-->
<a href="#" data-toggle="modal" data-target="#basicModal"><button type="button" class="btn btn-sm btn-success"><strong>+</strong></button></a>
</h3>
            </div>
            <div class="panel-body">
<?php if (isset($TestType)) { require_once "panel/test_table.php"; } 
       else { echo "<h1>Test Type not defined.</h1>";} 
?>
    </div></div></div></div>

    </div> <!-- /container -->
 <div class="modal fade" id="basicModal" tabindex="-1" role="dialog" aria-labelledby="basicModal" aria-hidden="true">
  <div class="modal-dialog modal-lg">
  <div class="modal-content">
  <div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title" id="myModalLabel">Create a test | Choose number of variants:</h4>
    </div>
    <form class="test" name="test" action="aev_test_1.php">
    <div class="modal-body">
    <div class="form-group">
      <label class="label" for="variant_list" >Choose the number of test variant to be added in the test:</label>	
       <select class="form-control" id="variant_list" name="num_var">
<?php if ($TestType == "ABTest") { ?>
        <option value="1">1</option>
	<option value="2">2</option>
	<option value="3">3</option>
<?php } else { ?> 
        <option value="2">2</option>
        <option value="3">3</option>
        <option value="4">4</option>
        <option value="5">5</option>
        <option value="6">6</option>
        <option value="7">7</option>
        <option value="8">8</option>
<?php } ?>
       </select>
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

	<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>

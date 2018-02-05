<!-- Test Outcome Panel -->
  <div class="col-xs-6">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">Test Outcome</h3>
  </div>
  <div class="panel-body">
  <form  class="ajax_form" type="post"> 
  <div class="form-group">
  <input type='hidden' name='form' value='terminate_exp'>
  <br/>
<?php if ( ( $test_data['state_id'] == "1") || ( $test_data['state_id'] == "2") || ( $test_data['state_id'] == "3") ) { ?>
  <label for="sel1">Pick a Winner: &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Picking a winner fixes the experiment to a single variant or the control group. `'></span></label><br/><br/><br/>
  <select class="form-control" id="sel1" name="final_variant">
<?php for ( $i = 0; $i < $nR; $i++ ) {
  echo "<option>".$result[$i]['name']."</option>";
} ?>
  </select><br/>
<?php } else { ?>
  <h3>Winner Variant:
<?php for ( $i = 0; $i < $nR; $i++ ) {
  if ($result[$i]['is_final'] == 1) { echo $result[$i]['name']; }
} ?>
</h3><br/><br/><br/>
<?php } ?>
<?php if ( ( $test_data['state_id'] == "1") || ( $test_data['state_id'] == "2") || ( $test_data['state_id'] == "3") ) { ?>
  <input type="hidden" name="TestName" value="<?php echo $test_name; ?>">
  <input type="hidden" name="modifier" value="<?php echo $modifier; ?>">
  <button class="btn btn-lg btn-danger btn-block" type="button" data-toggle="modal" data-target="#confirmTerminate" data-title="Terminate the Test" data-message="Are you sure you want to terminate this test? You cannot reinstate the test."
<?php if (($test_data['state_id'] == "2") || ($test_data['state_id'] == "3") ) {} else { echo "disabled";} 
if (($test_data['is_private'] == 1) && ($user_id != $creator)){echo "disabled";}
?>
  >FIX TO A WINNER</button>
<?php } ?>
  <br/></form>
  </div>
  </div>
  </div>

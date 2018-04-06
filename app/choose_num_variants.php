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
<?php

if ($TestType == "ABTest")
	{ ?>
  <option value="1">1</option>
	<option value="2">2</option>
	<option value="3">3</option>
<?php
	}
else
	{ 
?> 
  <option value="2">2</option>
  <option value="3">3</option>
  <option value="4">4</option>
  <option value="5">5</option>
  <option value="6">6</option>
  <option value="7">7</option>
  <option value="8">8</option>
<?php
	} ?>
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

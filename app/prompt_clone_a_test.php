<div class="modal fade" id="CloneModal" tabindex="-1" role="dialog" aria-labelledby="basicModal" aria-hidden="true">
<div class="modal-dialog modal-lg">
<div class="modal-content">
<div class="modal-header">
  <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
  <h4 class="modal-title" id="CloneModalTitle"></h4>
</div>
<form class="test" name="test" >
<div class="modal-body">
<div class="alert hidden" role="alert" id="modalAlert"></div>
<div class="form-group">
<label for="CloneTestName" >Please enter Test Name of the clone:</label>	
<input type="text" value="" name="clone_name" id="CloneTestName" placeholder="Enter Clone Test Name">
<label for="TestID" ></label>	
<input type="hidden" id="TestID" name="TestID" value="" />
<label for="Creator" ></label>	
<input type="hidden" id="Creator" name="Creator" value="" />
</div>
</div>
<div class="modal-footer">
  <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
  <input class="btn btn-success" type="button" value="Clone the test" id="CloneModalSubmit">
</div>
</form>
</div>
</div>
</div>

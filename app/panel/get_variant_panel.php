<!---------------------------------------------------------------------------------------->
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">Get Variant from RTS</h3>
  </div>
  <div class="panel-body">
  <form  class="ajax_form" type='post'>
  <table>
  <tr>
  <td width="10%">UUID:</td>
  <td >
  <div class="input-group">
  <input type='hidden' name='form' value='get_variant'>
  <input class="form-control" id="uuid" name="uuid" maxlength="36" size="36" required><br/>
  <input type='hidden' name='TestName' value="<?php echo $test_name; ?>">
  <span class="input-group-btn">
  <input class="btn btn-success" type="submit" value="Get Variant" id="submit_form">
  </span>
  </div>
  </td>
  </tr>
  </table>	
  </form>
  </div>
  </div>
  </div>
  </div>

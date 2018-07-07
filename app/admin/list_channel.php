<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "includes/header_admin.php"; 
require_once "db_get_rows.php";
?>
<script src="js/add_channel.js"></script>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">LIST OF CHANNEL &nbsp; &nbsp;
<a href="#" data-toggle="modal" data-target="#AddChannel"><button type="button" class="btn btn-sm btn-info"><span class="glyphicon glyphicon-plus"></span>Channel</button></a>
</h3>

            </div>
            <div class="panel-body">
<?php require_once "../includes/error_div.php"; ?>
			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>S.No.</b></td><td><b>CHANNEL NAME</b></td></tr>
<?php
$admin = db_get_rows('channel');
$nR = count($admin); for ( $i = 0; $i < $nR; $i++ ) {
	echo "<tr><td>".$admin[$i]['id']."</td><td>".$admin[$i]['name']."</td></tr>";
    }
?>
</table>
            </div></div>
          </div>
        </div></div>
<!---------------------------------->
<! -- add channel -->			
    <div class="modal fade" id="AddChannel" tabindex="-1" role="dialog" aria-labelledby="AddChannel" aria-hidden="true">
    <div class="modal-dialog modal-lg">
    <div class="modal-content">
    <div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title" id="myModalLabel">Add Channel</h4>
    </div>
    <form class="test" type='post' id='addChannel'>
    <div class="modal-body">
    <div class="form-group">
    <label class="label" for="channel" >Channel:</label>
    <input class="form-control" id="channel" name="channel" required>
    </div>					
    </div>
    <div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
    <input class="btn btn-success" type="submit" value="Add Channel" id="submit">
    </div>
    </form>
    </div>
    </div>
    </div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

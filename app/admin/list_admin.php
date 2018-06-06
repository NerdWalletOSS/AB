<?php session_start();
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "../includes/header_admin.php"; 
require_once "db_get_rows.php";
?>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">LIST OF ADMIN USERS
<a href="#" data-toggle="modal" data-target="#AddAdmin"><button type="button" class="btn btn-sm btn-info"><span class="glyphicon glyphicon-plus"></span>Admin</button></a>
</h3>

            </div>
            <div class="panel-body">
			<div class="table-responsive">
<table class="table table-striped" >
<tr><td><b>S.No.</b></td><td><b>USER NAME</b></td></tr>
<?php
$admin = db_get_rows('admin');
$nR = count($admin); for ( $i = 0; $i < $nR; $i++ ) {
	echo "<tr><td>".$admin[$i]['id']."</td><td>".$admin[$i]['name']."</td></tr>";
    }
?>
</table>
            </div></div>
          </div>
        </div></div>
<!----------------------------------------->			
    <div class="modal fade" id="AddAdmin" tabindex="-1" role="dialog" aria-labelledby="AddAdmin" aria-hidden="true">
    <div class="modal-dialog modal-lg">
    <div class="modal-content">
    <div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title" id="myModalLabel">Add admin</h4>
    </div>
    <form class="test" name="test" action="add_admin_process.php">
    <div class="modal-body">
    <div class="form-group">
    <label class="label" for="admin" >Admin Name:</label>
    <input class="form-control" id="admin" name="admin_name" required>
    </div>					
    </div>
    <div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
    <input class="btn btn-success" type="submit" value="Add Admin" id="submit">
    </div>
    </form>
    </div>
    </div>
    </div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

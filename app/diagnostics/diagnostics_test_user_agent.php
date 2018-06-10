<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "chk_url.php";
require_once "includes/header_diagnostics.php"; 
//-----------------------------------------------------------
?>
<script src="js/add_admin.js"></script>
<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">TEST USER AGENT
</h3>

            </div>
            <div class="panel-body">
<?php
$UserAgent = $_GET['UserAgent'];
$rslt = chk_url( $UserAgent );
var_dump($rslt);
if (!$rslt) {
echo "User Agent unavailable";
}
?>
</table>
          </div>
        </div></div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

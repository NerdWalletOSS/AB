<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
require_once "get_url.php";
require_once "includes/header_diagnostics.php"; 
//-----------------------------------------------------------
?>
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
$url = 'ClassifyUA?UserAgent='.urlencode($UserAgent);
$http_code = 0;
$rslt = "";
$data = get_url( 'localhost', '8000',$url, $http_code, $rslt );
var_dump($data);
if (!$data) {
echo "User Agent unavailable";
} else {
print("<pre>".print_r($rslt,true)."</pre>");
}
?>
</table>
          </div>
        </div></div>
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

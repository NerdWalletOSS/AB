<?php session_start();
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php");

require_once "db_get_rows.php"; 
require_once "get_url.php";
require_once "includes/header_diagnostics.php"; 
?>

<div class="container">
	<div class="row" >
         <!--<div class="col-xs-6">-->
          <div class="panel panel-primary">
            <div class="panel-heading">
              <h3 class="panel-title">QUERY RTS &nbsp; &nbsp;

</h3>

            </div>
            <div class="panel-body">

			<div class="table-responsive">
<table class="table table-striped" >
<tbody>
<form  id='QueryRTS' class="form-signin" action="" method="POST"></form>
<input form="QueryRTS" type='hidden' name='server' value='<?php echo $_SERVER['SERVER_NAME']; ?>' >
	<tr>
		<td >Test Type: 
      <select form="QueryRTS" name='TestType'>";
<?php 
$test_type    = db_get_rows('test_type');
var_dump($test_type);
$ntt = count($test_type);
for ( $i = 0; $i < $ntt; $i++ ) { 
  echo "<option value='".$test_type[$i]['name']."'"; 
  echo ">".$test_type[$i]['name']."</option>";
} ?>
  </select>
   </td>

		<td >Query Type: 
      <select form="QueryRTS" name='QueryType'>
  <option value='Reload'>Reload</option>
  <option value='ListTests'>List Tests</option>
  <option value='TestInfo'>Test Information</option>
  </select>
   </td>

		<td >Test Name: <input form="QueryRTS" type='text' name='TestName' ></td>
	</tr>
<tr>
<td colspan="3"><input class="btn btn-lg btn-success btn-block" type="submit" form="QueryRTS"  value="Query"></td>
</tr>
<tr>
<td colspan="3">
<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
//-----------------------------------------------------------
$server = $_POST['server'];
$QueryType = $_POST['QueryType'];
$TestType = $_POST['TestType'];
if (isset($_POST['TestName']) ) {$TestName = $_POST['TestName'];}

switch ( $QueryType ) {
  case "Reload" : 
  $url = $QueryType;
  break;
  case "ListTests" : 
  $url = $QueryType."?TestType=".$TestType;
  break;
  case "TestInfo" : 
  $url = $QueryType."?TestType=".$TestType."&TestName=".$TestName;
  break;
  default : 
      // HANDLE ERROR PROPERLY TODO
  break;
 }



$http_code = 0;
$rslt = "";
$destination = "";
$data = get_url( $server, '8000',$url, $http_code, $rslt, $destination );
echo "<strong>QUERY: ".$url."</strong><br/>";
echo "<pre><code>".$rslt."</code></pre>";
}
?>
</td>
</tr>
</tbody>
</table>
            </div></div>
          </div>
        </div></div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>

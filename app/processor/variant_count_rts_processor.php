<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers/");

require_once "db_get_rows.php";
require_once "db_get_test.php";
require_once "get_url.php";


// Extract TestID
$id= $_GET['id'];

// Get Test Data
$T = db_get_test($id);
$TestName= $T['name'];
$n_var= $T['NumVariants'];

// Get RTS data
$url = 'TestInfo?TestType=XYTest&TestName='.$TestName;
$http_code = 0;
$rslt = "";
$data = get_url( 'localhost', '8000',$url, $http_code, $rslt, $destination );
//-----------------------------------------------------------
echo '
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">Variant Count Table</h3>
  </div>
  <div class="panel-body">';
?>
<?php if (!$data) { echo "No Information from RTS"; } else {
echo '
  <!-- ADD/EDIT FORM START  -->
  <table class="table table-striped table-condensed" style="space=5px">
  <tbody>
   <tr>
     <td>Variants\Devices</td>';

$result = json_decode($rslt);
for ( $i = 0; $i < $n_var; $i++ ) { 
  echo "<td>".$T['Variants'][$i]['name']."</td>";
} 
echo "</tr>";
echo "<tr>";
echo "<td></td>";
$result = json_decode($rslt);
foreach ($result as $k => $v) {
  if ($k == "Variants") {
    foreach ($v as $k1 => $v1) {
      foreach($v1 as $k2 => $v2) {
        if ($k2 == "count") {
        echo "<td>".$v2."</td>";
      }
    }
  }
}
}
echo "</tr>"; 
echo '
</tbody>
 </table>';
} ?>
</div>
</div>
</div>
</div>


  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">Ramp History</h3>
  </div>
  <div class="panel-body">
  <div id="ramp" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
  <table class="table table-striped">
  <thead>
  <tr>
  <th>Modified By</th>
  <th>Ramp Num</th>
  <th>Variant</th>
  <th>When Changed</th>
  <th>Distribution</th>
  </tr>
  </thead>
  <tbody>
<?php for ( $i = 0; $i < $num_rows_ramp; $i++ ) { 
  echo "<tr>";
  echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $res_ramp[$i]['modifier_id'] );  if ($rslt) {echo $rslt['name']; }?>
<?php echo "</td>
  <td>".$res_ramp[$i]['ramp_num']."</td>
  <td>";?><?php $rslt = db_get_row('variant', 'pk', $res_ramp[$i]['variant_id'] );  if ($rslt) {echo $rslt['name'];}?>
<?php echo "</td>
  <td>".$res_ramp[$i]['when_changed']."</td>
  <td>".$res_ramp[$i]['percentage']."</td>";
  echo "</tr>";
  }
  ?>
  </tbody>
  </table>
  </div>
  </div>
  </div>
  </div>

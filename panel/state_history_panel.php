<?php if ( $numRows > 0) { ?>
  <div class="row">
  <div class="col-xs-12">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">State History</h3>
  </div>
  <div class="panel-body">
  <table class="table table-striped">
  <thead>
  <tr>
  <th>Changed By</th>
  <th>Changed To</th>
  <th>When Changed</th>
  </tr>
  </thead>
  <tbody>
<?php for ( $i = 0; $i < $numRows; $i++ ) { 
  echo "<tr>";
  echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $res[$i]['modifier_id'] );  if($rslt) {echo $rslt['name'];}?>
<?php echo "</td><td><button type='button' class='btn btn-xs btn-"?>
<?php bootstrap_state_color($res[$i]['state_id'], $color, $state); echo $color; ?>
<?php echo "'>".$state."</button></td><td>".$res[$i]['when_changed']."</td>";
  echo "</tr>";
}
?>
  </tbody>
  </table>
  </div>
  </div>
  </div>
  </div>
<?php } ?>

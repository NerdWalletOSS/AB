<?php if( isset($_SESSION["err"]) ){echo '<div id="msg" align="center" class="msg" style="z-index:100011;font-family:Tahoma,Arial;">'.$_SESSION["err"].'</div>';
unset($_SESSION["err"]); } else { /* Do Nothing */ }

function action_state($state_id) {
  switch ($state_id) {
    case '1':
      return 'Publish';
    break;

    case '2':
      return 'Start';
    break;

    case '3':
      return 'Stop';
    break;

    case '4':
      return 'Archive';
    break;
  }
}
?>
<div id="show-data">
<!--<table id="myTable" class="table table-striped table-condensed" >
  <thead>
  <tr>
  <th>ID</th>
  <th>Name</br>&nbsp;</th>
  <th>State</br>&nbsp;</th>
  <th>Action</br>&nbsp;</th>
  </tr>
  </thead>-->
<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>Action</th><th>Edit/view</th> </tr></thead><tfoot> <tr><th>ID</  th><th>Name</th><th>Action</th><th>Edit/view</th></tr></tfoot>
  <tbody id="TableData">
<?php $nR = count($result); for ( $i = 0; $i < $nR; $i++ ) { //bootstrap_state_color($result[$i]['state_id'], $color, $state);
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['id']."</td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['name']."</td>";
  echo "<td><a href='processor/set_state_processor.php?TestID=".$result[$i]['id']."&state_id=".$result[$i]['state_id']."'><button type='button' class='btn btn-primary btn-xs'>".action_state($result[$i]['state_id'])."</button></td>";

 echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='aev_test_1.php?TestID=".$result[$i]['id']."' >Edit/View</a></td>";
} ?>

  </tbody>
  </table>
</div>

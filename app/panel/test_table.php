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
<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</th><th>Name</th><th>Check Test</th><th>Action</th> </tr></thead>
<tfoot> <tr><th>ID</th><th>Name</th><th>Check Test</th><th>Action</th></tr><tfoot>
  <tbody id="TableData">
<?php $nR = count($result); for ( $i = 0; $i < $nR; $i++ ) {
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='aev_test_1.php?TestID=".$result[$i]['id']."' >".$result[$i]['id']."</a></td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='aev_test_1.php?TestID=".$result[$i]['id']."' >".$result[$i]['name']."</a></td>";
  if (($result[$i]['state_id'] == 3)|| ($result[$i]['state_id'] == 4)) {
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='processor/check_test_processor.php?TestName=".$result[$i]['name']."' >
<button type='button' class='btn btn-warning btn-xs'>Check Test</button></a></td>";
}
  if ($result[$i]['state_id'] == 3) {
echo "<td><a href='fix_to_a_winner.php?TestID=".$result[$i]['id']."'><button type='button' class='btn btn-primary btn-xs'>".action_state($result[$i]['state_id'])."</button></a></td>";
  } else {
  echo"<td>";
  echo "<a href='processor/set_state_processor.php?TestID=".$result[$i]['id']."&state_id=".$result[$i]['state_id']."'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>".action_state($result[$i]['state_id'])."</button></a>";
 
  if ($result[$i]['state_id'] == 1) {
    echo "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=".$result[$i]['id']."&state_id=".$result[$i]['state_id']."&action=delete"."'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Delete</button></a>";
  }
  if ($result[$i]['state_id'] == 4) {
    echo "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=".$result[$i]['id']."&state_id=".$result[$i]['state_id']."&action=resurrect'"."><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Resurrect</button></a>";
  }
  echo "</td>";
}
} ?>



  </tbody>
  </table>
</div>

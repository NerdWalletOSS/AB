<?php if( isset($_SESSION["err"]) ){echo '<div id="msg" align="center" class="msg" style="z-index:100011;font-family:Tahoma,Arial;">'.$_SESSION["err"].'</div>';
unset($_SESSION["err"]); } else { /* Do Nothing */ }?>
<table id="myTable" class="table table-striped table-condensed" >
  <thead>
  <tr>
  <th>ID</th>
  <th>Name</br>&nbsp;</th>
  <th>State</br>&nbsp;</th>
  <th>Action</br>&nbsp;</th>
  </tr>
  </thead>
  <tbody>
<?php for ( $i = 0; $i < $nR; $i++ ) { //bootstrap_state_color($result[$i]['state_id'], $color, $state);
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['id']."</td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='exp_dashboard.php?id=".$result[$i]['id']."' >".$result[$i]['name']."</a></td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['state_id']."</td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>Edit, View, Change State</td>";
} ?>

  </tbody>
  </table>

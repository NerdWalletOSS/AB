<?php if( isset($_SESSION["err"]) ){echo '<div id="msg" align="center" class="msg" style="z-index:100011;font-family:Tahoma,Arial;">'.$_SESSION["err"].'</div>';
unset($_SESSION["err"]); } else { /* Do Nothing */ }?>
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
<table id="TestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></thead>
<tfoot> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></tfoot>
  <tbody id="TableData">
<?php $nR = count($result); for ( $i = 0; $i < $nR; $i++ ) { //bootstrap_state_color($result[$i]['state_id'], $color, $state);
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['id']."</td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['name']."</td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a class='confirmLink' href='processor/set_state.php?id=".$result[$i]['state_id']."'><button onclick='confirm()' type='button' class='btn btn-success btn-lg'>".$result[$i]['state_id']."</button></a></td>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='aev_test_1.php?TestID=".$result[$i]['id']."' >Edit/View,</a> ,
  Change State</td>";
} ?>

  </tbody>
  </table>
</div>

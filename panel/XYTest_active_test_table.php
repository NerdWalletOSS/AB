<?php if( isset($_SESSION["err"]) ){echo '<div id="msg" align="center" class="msg" style="z-index:100011;font-family:Tahoma,Arial;">'.$_SESSION["err"].'</div>';
unset($_SESSION["err"]); } else { /* Do Nothing */ }?>
<?php

//var_dump($result);

?>
<table id="myTable" class="table table-striped table-condensed" >
  <thead>
  <tr>
  <th>Test Name</br>&nbsp;</th>
  <!--<th>URL</th>-->
  <th>Owner</br>&nbsp;</th>
  <th >&nbsp;&nbsp;Channel</th>
  <!--<th class="hidden-xs">Created At</th>-->
  <th class='hidden-xs'>Updated At</br>&nbsp;</th>
  <!--<th>URLs</th>-->
  <th>Status</br>&nbsp;</th>
  <th>Action</br>&nbsp;</th>
    <th>Device Spec</br>&nbsp;</th>
  </tr>
  </thead>
  <tbody>
<?php for ( $i = 0; $i < $nR; $i++ ) { bootstrap_state_color($result[$i]['state_id'], $color, $state);
  $channel=db_get_row('channel', 'pk', $result[$i]['channel_id']); if ($channel['name'] == "") {$channel['name'] = 'None';}
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='exp_dashboard.php?id=".$result[$i]['pk']."' >".$result[$i]['name']."</a></td>
";?>
<?php
  //echo "<td class='hidden-xs' style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['description']."</td>";
  echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $result[$i]['creator_id'] );
  $abtest_id = $rslt['pk'];
  echo $test_name = $rslt['name'];?><?php echo "</td>
  <td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$channel['name']."</td>
  <!--<td class='hidden-xs'>".substr($result[$i]['date_created'], 0, 10)."</td>-->
  <td class='hidden-xs'>".substr($result[$i]['most_recent_update'], 0, 20)."</td>
  <!--<td>".$result[$i]['num_variants']."</td>-->
  <td class='text-"?><?php  echo $color; ?><?php echo "'><b>".$state."</b></td>
  <td>";?>
<?php if ($result[$i]['state_id'] == 1) {
bootstrap_state_action($result[$i]['state_id'], $color, $action, $action_name);?>
<?php echo "<a data-href='take_action.php?test_name=".$result[$i]['name']."&amp;modifier=";?>
<?php echo $modifier; ?>
<?php echo "&amp;action=".$action."' id='action' role='button' class='btn btn-xs btn-"?><?php echo $color; ?><?php echo "' data-toggle='confirmation'>".$action_name."</a></td>";
} elseif ($result[$i]['state_id'] == 2) {
bootstrap_state_action($result[$i]['state_id'], $color, $action, $action_name);?>
<?php echo "<a href='exp_dashboard.php?id=".$result[$i]['pk']."' id='action' role='button' class='btn btn-xs btn-"?><?php echo $color; ?>
<?php echo "'>".$action_name."</a>";

} elseif ($result[$i]['state_id'] == 4) {
bootstrap_state_action($result[$i]['state_id'], $color, $action, $action_name);?>
<?php echo "<a href='exp_dashboard.php?id=".$result[$i]['pk']."' id='action' role='button' class='btn btn-xs btn-"?><?php echo $color; ?>
<?php echo "'>Next Step</a>

</td>";
}
?>
<?php
   $is_device_specific = get_device_specific($test_name, $abtest_id);
   #var_dump($is_device_specific);
    $test_data = db_get_test($abtest_id);
	$creator_id = $test_data['creator_id'];
	$creator_row = db_get_row( 'admin','pk', $creator_id);
    $creator = $creator_row['name'];
	$is_private = $test_data['is_private'];
    if (($is_device_specific == 0) && ($result[$i]['state_id'] == 1) ){
      if (($is_private == 0) || ($modifier == $creator) ){
     echo "<td>
     <a href='set_device_specific_dist.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-primary'>Set</a>
     </td>";
	  }
} elseif(($is_device_specific == 0) && ($result[$i]['state_id'] == 2) ){

 echo "<td class='text-warning'>
<a href='set_device_specific_dist.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-warning'>Update</a>
      </td>";
}
else {
 echo "<td class='text-warning'>
<a href='set_device_specific_dist.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-info'>View</a>
      </td>";
}
}
echo "</tr>";
?>
  </tbody>
  </table>

  <table id="myTable" class="table table-striped table-condensed" >
  <thead>
  <tr>
  <th>Test Name</br>&nbsp;</th>
  <!--<th>URL</th>-->
  <th>Owner</br>&nbsp;</th>
  <th >&nbsp;&nbsp;Channel</th>
  <!--<th class="hidden-xs">Created At</th>-->
  <th class='hidden-xs'>Updated At</br>&nbsp;</th>
    <th>Device Spec</br>&nbsp;</th>
  <!--<th>URLs</th>-->
  </tr>
  </thead>
  <tbody> 
<?php for ( $i = 0; $i < $nR; $i++ ) { bootstrap_state_color($result[$i]['state_id'], $color, $state);
  $channel=db_get_row('channel', 'pk', $result[$i]['channel_id']); if ($channel['name'] == "") {$channel['name'] = 'None';}
  echo "<tr>";
    echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a href='exp_dashboard.php?id=".$result[$i]['pk']."' >". $result[$i]['pk'] . " :" . $result[$i]['name']."</a></td>
";?>
<?php 
  //echo "<td class='hidden-xs' style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$result[$i]['description']."</td>";
  echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $result[$i]['creator_id'] );
  echo $rslt['name'];?><?php echo "</td>
  <td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>".$channel['name']."</td>
  <!--<td class='hidden-xs'>".substr($result[$i]['date_created'], 0, 10)."</td>-->
  <td class='hidden-xs'>".substr($result[$i]['most_recent_update'], 0, 10)."</td>
  <td class='text-warning'>
<a href='set_device_specific_dist.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-danger'>View</a>
      </td>";
  //<td>".$result[$i]['num_variants']."</td>-->";
}
?>
  </tbody>
  </table>

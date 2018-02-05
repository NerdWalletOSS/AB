  <table id="myTable" class="table table-striped table-condensed" style="word-wrap: break-word">  
  <thead>  
  <tr>  
  <th>Test Name</th>  
  <th>Owner</th>  
  <th class="hidden-xs">Created At</th>  
  <th class='hidden-xs'>Updated At</th>  
  <th>Variants</th>
  <th>Filter</th>
  </tr>  
  </thead>  
  <tbody> 
<?php for ( $i = 0; $i < $nR; $i++ ) { bootstrap_state_color($result[$i]['state_id'], $color, $state);
  echo "<tr>";
  echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a class='post' href='"?><?php echo "exp_dashboard.php?id=".$result[$i]['pk']."' data-toggle='tooltip' data-placement='right' title='".$result[$i]['description']."'>".$result[$i]['name']."</a></td>
";?>
<?php
  echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $result[$i]['creator_id'] );
  echo $rslt['name'];?><?php echo "</td>
  <td class='hidden-xs'>".$result[$i]['date_created']."</td>
  <td class='hidden-xs'>".$result[$i]['most_recent_update']."</td>
  <td>".$result[$i]['num_variants']."</td>
  <td class='text-warning'>
    <a href='filter.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-danger'>View</a>
  </td>";
  }
?>
  </tbody>  
  </table>  

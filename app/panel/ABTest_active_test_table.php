	<table id="myTable" class="table table-striped table-condensed" style="word-wrap: break-word">  
        <thead>  
          <tr>
            <th>ID</th> 
            <th>Name</th>  
            <!--<th>Owner</th>  
            <th class="hidden-xs">Created At</th>  
	     		<th class='hidden-xs'>Updated At</th>  
            <th>Variants</th>--> 
            <th>State</th> 
	    		<th>Action</th>	
	       	<!--<th>Filter</th>-->
          </tr>  
  
        </thead>  
        <tbody> 
		<?php for ( $i = 0; $i < $nR; $i++ ) { bootstrap_state_color($result[$i]['state_id'], $color, $state);
		echo "<tr>";
		echo "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><a class='post' href='"?><?php echo "exp_dashboard.php?id=".$result[$i]['pk']."' data-toggle='tooltip' data-placement='right' title='".$result[$i]['description']."'>".$result[$i]['name']."</a></td>
		";?><?php 
		echo "<td>"; ?><?php $rslt = db_get_row('admin', 'pk', $result[$i]['creator_id'] );
		  $abtest_id = $rslt['pk'];
        echo $test_name = $rslt['name'];?><?php echo "</td>
		<td class='hidden-xs'>".$result[$i]['date_created']."</td>
		<td class='hidden-xs'>".$result[$i]['most_recent_update']."</td>
		<td>".$result[$i]['num_variants']."</td>
		<td class='text-"?><?php  echo $color; ?><?php echo "'><b>".$state."</b></td>
		<td>";?>
		<?php if (($result[$i]['state_id'] == 1) || ($result[$i]['state_id'] == 4) ){ 
	        bootstrap_state_action($result[$i]['state_id'], $color, $action, $action_name);?>
		<?php echo "<a data-href='take_action.php?test_name=".$result[$i]['name']."&amp;modifier=";?>
		<?php  echo $modifier; ?>
		<?php echo "&amp;action=".$action."' id='action' role='button' class='btn btn-xs btn-"?><?php echo $color; ?>
		<?php echo "' data-toggle='confirmation'>".$action_name."</a></td>";
		} elseif ($result[$i]['state_id'] == 2) { 
	        bootstrap_state_action($result[$i]['state_id'], $color, $action, $action_name);?>
		<?php echo "<a href='exp_dashboard.php?id=".$result[$i]['pk']."' id='action' role='button' class='btn btn-xs btn-"?><?php echo $color; ?><?php echo "'>".$action_name."</a>
		</td>";}?>
		
		<?php 
      $is_device_specific = get_device_specific($test_name, $abtest_id);
      $test_data = db_get_test($abtest_id);
	   $creator_id = $test_data['creator_id'];
	   $creator_row = db_get_row( 'admin','pk', $creator_id);
      $creator = $creator_row['name'];
	   $is_private = $test_data['is_private'];
      if (($is_device_specific == 0) && ($result[$i]['state_id'] == 1) ){ 
      if (($is_private == 0) || ($modifier == $creator) ){ 
      echo "<td>
      <a href='filter.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-primary'>Set</a>
      </td>";
	   }
      }  else { 
  echo "<td class='text-warning'>
<a href='filter.php?id=".$result[$i]['pk']."' id='actionD' role='button' class='btn btn-xs btn-warning'>View</a>
      </td>";
}
      }
      echo "</tr>";
		?>
        </tbody>  
      </table>  

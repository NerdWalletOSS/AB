<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "dbconn.php";
require_once "db_get_row.php";

function tb2csv(
  $datadir,
  $table
) 
{
  $dbh = dbconn("/opt/abadmin/db.json");

  $opfile = "$datadir/".$table.".csv";
  #echo "exporting $table into $opfile \n";
  $ofp = fopen($opfile, "w");
  if ( !$ofp ) { echo "Could not open output file [$opfile]\n"; exit(1); }

  // Fetch Record from Database
  $sql = mysql_query("select * from $table");
  $num_columns 	= mysql_num_fields($sql);

  // Print header line  = names of fields 
  $col_names = array($num_columns);
  $output = "";
  $colidx = 0;
  for ($i = 0; $i < $num_columns; $i++) {
    $col_name = mysql_field_name($sql, $i);
    $col_type = mysql_field_type($sql, $i);
    if ( $col_name == "description" ) { continue; }
    if ( $colidx > 0 ) { $output .= ","; }
    $output .= "\"$col_name\"";
    $col_names[$colidx] = $col_name;
    $colidx++;
  }
  $num_columns = $colidx;
  sort($col_names);
  $str_col_names = "";
  for ($i = 0; $i < $num_columns; $i++) {
    if ( $i == 0 ) { 
    $str_col_names .= $col_names[$i];
    }
    else {
    $str_col_names .= "," . $col_names[$i] ;
    }
  }
  $nsql = mysql_query(" select $str_col_names from $table");
  fprintf($ofp, "$str_col_names\n"); 

  // Get Records from the table

  date_default_timezone_set('America/Los_Angeles');

  $row_idx = 0;
  while ( $row = mysql_fetch_array($nsql) ) {
    $colidx = 0;
    $output = "";
    for ($i = 0; $i < $num_columns; $i++) {
      $col_name = $col_type = $cell_val = NULL;
      $col_name = mysql_field_name($nsql, $i);
      if ( $col_name == "description" ) { 
        echo "ERR: FILE:" . __FILE__ . " LINE: " . __LINE__ . "\n";
        return false; 
      }
      $col_type = mysql_field_type($nsql, $i);
      if ( $colidx > 0 ) { $output .= ","; }
      $cell_val = $row[$i];
      $mod_cell_val = $cell_val; // default behavior 
      // May need to modify cell value for certain cases.
      if ( $col_type == "datetime" ) {
        $dateTime = new DateTime($cell_val);
        $dateTime->format('Y-m-d h:m:s');
        $dateTime->setTimeZone(new DateTimeZone('America/Los_Angeles'));
        $mod_cell_val = $dateTime->format('U');
        /*
          echo ($table . ":" . $col_name . " = " . $mod_cell_val . 
          " --> " . $dateTime->format('U') . "\n" );
       */
      }
      if ( $col_name == "state_id" ) { 
        $xrow = db_get_row("state", "pk", $cell_val);
        $mod_cell_val = $xrow['name'];
      }
      //------------------------------------------------------
      $output .= '"' . $mod_cell_val . '"';
      $colidx++;
    }
    /*
    if ( $table == "state_history" ) { 
      var_dump($row);
      echo "when_changed = " . $row["when_changed"] . "\n";
    }
     */
    fprintf($ofp, "%s\n", $output);
    $row_idx++;
    $row = NULL;
  }
  fclose($ofp);
  return true;
}
?>

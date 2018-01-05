<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../tcc/");
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_set_row.php";
require_once "db_get_variants.php";

function chk_set_ramp(
  &$test_name, 
  &$test_type, 
  $variant_spec, // e.g., "Red:10 Blue:20"
  &$modifier,
  &$is_change,
  &$V
)
{
  $V = null;
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $configs = $GLOBALS['CONFIGS'];
  if ( !$configs ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $dbh = dbconn();  
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------
  $test_type = trim($test_type);
  $rslt = db_get_row("test_type", "name", $test_type);
  if ( !$rslt ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $test_type_id = $rslt['pk']; 
  //-------------------------------------------------
  $started_id = name_to_id("state", "name", "started");
  $dormant_id = name_to_id("state", "name", "dormant");
  //-------------------------------------------------
  $test_name = trim($test_name); 
  $T = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id and ( ( state_id = $started_id ) or ( state_id = $dormant_id ) ) ");
  if ( !$T )  { 
    $GLOBALS["err"] .= "Invalid test [$test_name]";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------
  $abtest_id     = $T['pk'];
  $num_variants  = $T['num_variants'];
  $is_private    = $T['is_private'];
  $is_sequential = $T['is_sequential'];
  $creator_id    = $T['creator_id'];
  //-----------------------------------------------------------
  $A = db_get_row("admin", "name", $modifier);
  if (!$A ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid admin [$modifier] "; return false;
  }
  $modifier_id = $A['pk'];
  if ( ( $is_private == 1 ) && ( $modifier_id != $creator_id ) ) {
    $GLOBALS["err"] .= "User [$modifier] not creator\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-----------------------------------------------------------
  $variant_spec = trim($variant_spec);
  if ( $variant_spec == "" ) {
    $GLOBALS["err"] .= "Empty variant_spec [$variant_spec] "; 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  // START: Create arrays names/percs which contain the names and desired 
  // percentages of the variants
  $newV = explode(" ", $variant_spec);
  $sum_perc = 0;
  $nV = 0;
  foreach ( $newV as $y ) {
    $Z = explode(":", $y);
    if ( count($Z) != 2 ) {
      $GLOBALS["err"] .= "Invalid variant_spec [$variant_spec]\n"; 
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    // Check variant name
    $name = $Z[0];
    if ( $name == "" ) { 
      $GLOBALS["err"] .= "Invalid variant_spec [$variant_spec]\n"; 
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    for ( $i = 0; $i < $nV; $i++ ) {
      if ( $name == $names[$i] ) {
        $GLOBALS["err"] .= "Duplicate variant [$name]\n"; 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    }
    $names[$nV] = $name;
    // Check percentage
    $percentage = $Z[1];
    // TODO P3: Why does is_int() not work? 
    if (
      ( !is_numeric($percentage) ) || 
      ( $percentage < 0 ) || 
      ( $percentage > 100 ) ) {
        $GLOBALS["err"] .= "Invalid variant_spec [$variant_spec]\n"; 
        $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
        return false;
      }
    if ( $test_type == "ABTest" ) { 
      if ( $is_sequential == 1 ) { 
        // This test does not apply
      }
      else {
        if ( $name != "Control" ) { 
          if ( $percentage > (100.0 / ($num_variants-1)) ) {
            $GLOBALS["err"] .= "Invalid variant_spec [$variant_spec] "; 
            $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
            return false;
          }
        }
      }
    }
    $percs[$nV] = $percentage;
    $sum_perc += $percs[$nV];
    $nV++;
  }
  if ( $sum_perc != 100 ) { 
    $GLOBALS["err"] .= "Invalid variant_spec [$variant_spec]\n"; 
    $GLOBALS["err"] .= "Percentages = ($sum_perc) need to add up to 100\n"; 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false;
  }
  if ( $nV != $num_variants ) { 
    $GLOBALS["err"] .= "Expected $num_variants variants. Got $nV\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
    return false;
  }
  ##C \begin{enumerate}
  ##C Check that exactly one variant has 100 
  if ( ( $is_sequential ) && ( !chk_is_sequential($percs, $nV) ) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  ##C \end{enumerate}
  //---------------------------------
  $is_change = false;
  $V = array($nV); 
  for ( $i = 0; $i < $nV; $i++ ) {
    $vrow = db_get_row("variant", "name", $names[$i], 
      " and abtest_id = $abtest_id ");
    if (!$vrow ) { 
      $GLOBALS["err"] .= "Invalid variant {$names[$i]} \n";
      $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__. "\n";
      return false;
    }
    $v['pk']         = $vrow['pk'];
    $v['name']       = $vrow['name'];
    $v['percentage'] = $percs[$i];
    if ( $percs[$i] != $vrow['percentage'] ) { 
      $is_change = true;
    }
    $V[$i] = $v;
    if ( $names[$i] != $vrow['name'] ) { 
      go_BYE(-1);
    }
  }
  return true;
}
?>

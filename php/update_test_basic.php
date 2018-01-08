<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'lkp.php';
require_once 'make_seed.php';
require_once 'get_time_usec.php';
require_once 'aux_chk_name.php';
require_once 'aux.php';

function update_test_basic(
  $str_inJ
)
{
  $outJ['status'] = "error";
  $outJ['stdout'] = "";
  $outJ['stderr'] = "";
  // START Check inputs
  assert(isset($str_inJ));
  assert(is_string($str_inJ));
  $inJ = json_decode($str_inJ); assert(!is_null($inJ));
  $test_id   = get_json($inJ, 'TestID'); 
  $test_name = get_json($inJ, 'TestName'); 
  $test_type = get_json($inJ, 'TestType'); 
  $test_dscr = get_json($inJ, 'TestDescription'); 
  $updater   = get_json($inJ, 'updater');
  $variants  = get_json($inJ, 'Variants');
  assert(is_array($variants));
  $nV = count($variants);
  assert($nV > 0 );
  assert($nV >= lkp('configs', "min_num_variants"));
  assert($nV <= lkp('configs', "max_num_variants"));

  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= lkp("configs", "max_len_test_dscr"));
  }
  assert(aux_chk_name($test_name), "test name is invalid");
  assert(strlen($test_name) <= lkp("configs", "max_len_test_name"));
  $test_type_id = lkp("test_type", $test_type);
  $updater_id   = lkp("admin", $updater);
  $dormant_id   = lkp("state", "dormant");

  $variant_ids   = array($nV);
  $variant_names = array($nV);
  $variant_percs = array($nV);
  $vidx = 0;
  foreach ( $variants as $v ) { 
    $vname = $v->{'Name'};
    assert(isset($vname));
    assert(is_string($vname));

    $vid = $v->{'ID'};
    assert(isset($vid));
    assert(is_string($vid));
    $vid = intval($vid);
    assert($vid > 0);

    $perc = $v->{'Percentage'};
    assert(isset($perc));
    assert(is_string($perc));
    $perc = floatval($perc);
    
    $variant_names[$vidx] = $vname;
    $variant_ids[$vidx]   = $vid;
    $variant_percs[$vidx] = $perc;
    $vidx++;
  }
  assert(is_unique($variant_names));
  
  foreach ( $variant_names as $v ) {
    assert(aux_chk_name($v), "variant name is invalid");
    assert(strlen($v) <= lkp("configs", "max_len_variant_name"));
  }
  foreach ( $variant_percs as $p ) {
    assert(is_float($p), "percentage must be a number");
    assert($p >=   0, "percentage must not be negative");
    assert($p <= 100, "percentage cannot exceed 100");
  }
  assert(is_good_percs($percentages));

  assert(is_test_name_unique($test_name, $test_type), 
    "test name [$test_name] not unique");
  // STOP Check inputs
  //----------------------------------------------------
  $t = db_get_test($test_id);
  assert($t, "No test [$test_name] of type [$test_type]");
  $state = lkp("state", $t['state_id'], true); //reverse lkp

  $d_update = get_date(); 
  $t_update = get_time_usec(); 
  $X1['description']  = $test_dscr;
  $X1['d_update']     = $d_update;
  $X1['t_update']     = $t_update;
  $X1['updater_id']   = $updater_id;
  $X1['name']         = $test_name;

  // Can change some stuff only when dormant 
  if ( $state == "dormant" ) { 
    $X1['name']         = $test_name;
    switch ( $test_type ) {
    case "ABTest" :
      $bin_type_id = lkp("bin_type", "c_to_v_ok_v_to_c_ok_v_to_v_not_ok");
      break;
    case "XYTest" :
      $bin_type_id = lkp("bin_type", "free_for_all");
      break;
    default : 
      assert(null, "Invalid test type $test_type");
      break;
      assert(isset($bin_type_id));
      $X1['bin_type_id'] = $bin_type_id;
    }
  }
  //-----------------------------------------------
  $dbh = dbconn(); assert(isset($dbh)); 
  try {
    $dbh->beginTransaction();
    mod_row("test", $X1, "where id = $test_id ");
    $X2['t_update'] = $t_update;
    $X2['d_update'] = $d_update;
    //-------------------------------------------------------
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X2['percentage']  = $variant_percs[$i];
      // Can change some stuff only when dormant 
      if ( $state == "dormant" ) { 
        $X2['name']        = $variant_names[$i];
      }
      mod_row("variant", $X2, "where id = " . $variant_ids[$i]);
    }
    //------------------------------------------
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //------------------------------------------
  $outJ["status"] = "ok";
  $outJ["test_id"] = $test_id;
  return $outJ;
}

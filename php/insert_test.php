<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once 'is_test_name_unique.php';
require_once 'insert_row.php';
require_once 'lkp.php';
require_once 'make_seed.php';
require_once 'get_time_usec.php';
require_once 'aux_chk_name.php';
require_once 'aux.php';

function insert_test(
  $str_inJ
)
{
  $outJ['status'] = "error";
  $outJ['stdout'] = "";
  $outJ['stderr'] = "";
  // START Check inputs
  if ( empty($str_inJ) ) { 
    $_SESSION['stderr'] = "No input provided";
    $_SESSION['loc'] = __FILE__ . ":" . __LINE__;
    goto ERR;
  }
  rs_assert($str_inJ, "no input provided", 403);
  rs_assert(is_string($str_inJ), "input not string", 403);
  $inJ = json_decode($str_inJ); rs_assert($inJ, "invalid JSON");
  $test_name = get_json($inJ, 'TestName'); 
  $test_type = get_json($inJ, 'TestType'); 
  $test_dscr = get_json($inJ, 'TestDescription'); 
  $creator   = get_json($inJ, 'Creator');
  $variants  = get_json($inJ, 'Variants');
  assert(is_array($variants));
  $nV = count($variants);
  assert($nV > 0 );

  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= lkp("configs", "max_len_test_dscr"));
  }
  rs_assert(is_good_test_name($test_name, $test_type))

  $test_type_id = lkp("test_type", $test_type);
  $creator_id   = lkp("admin", $creator);
  $draft_id     = lkp("state", "draft");

  $variant_names = array($nV);
  $variant_percs = array($nV);
  $vidx = 0;
  foreach ( $variants as $v ) { 
    $name = $v->{'Name'};
    assert(isset($name));
    $perc = $v->{'Percentage'};
    assert(isset($perc));
    assert(is_string($perc));
    $perc = floatval($perc);
    
    $variant_names[$vidx] = $name;
    $variant_percs[$vidx] = $perc;
    $vidx++;
  }
  assert(is_good_variants($variant_names));
  assert(is_good_percs($variant_percs));
  // STOP Check inputs
  //----------------------------------------------------
  $test_id = -1;
  $d_create =  $d_update = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $X1['name']         = $test_name;
  $X1['description']  = $test_dscr;
  $X1['test_type_id'] = $test_type_id;
  $X1['seed']         = make_seed();
  $X1['external_id']  = $t_create;
  $X1['d_create']     = $d_create;
  $X1['t_create']     = $t_create;
  $X1['d_update']     = $d_update;
  $X1['t_update']     = $t_update;
  $X1['creator_id']   = $creator_id;
  $X1['updater_id']   = $creator_id;
  $X1['state_id']     = $draft_id;
  //-----------------------------------------------
  //-- In subsequent versions, we will allow user to pick $bin_type
  //-- For now, following is hard coded
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
  }
  assert(isset($bin_type_id));
  $X1['bin_type_id'] = $bin_type_id;
  //-----------------------------------------------
  $dbh = new_assert(dbconn(), 403, "unable to talk to DB");
  $dbh = dbconn(); assert(isset($dbh)); 
  try {
    $dbh->beginTransaction();
    $test_id = insert_row("test", $X1);
    $X2['test_id']  = $test_id;
    $X2['t_update'] = $t_update;
    $X2['d_update'] = $d_update;
    //-------------------------------------------------------
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X2['percentage']  = $variant_percs[$i];
      $X2['name']        = $variant_names[$i];
      insert_row("variant", $X2);
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
  $outJ["stdout"] = "Created test $test_name";
  $outJ["test_id"] = $test_id;
  return $outJ;
  ERR:
    return null;
}

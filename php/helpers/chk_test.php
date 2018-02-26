<?php
// Returns true if test is in healthy state
// Returns a string containing stuff to fix if not
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
}
require_once 'dbconn.php';
require_once 'insert_row.php'; // NO PROBLEM
require_once 'make_seed.php'; // NO PROBLEM
require_once 'get_time_usec.php'; // NO PROBLEM
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'db_get_row.php';
require_once 'mod_row.php';
require_once 'is_good_test_name.php';
require_once 'aux_chk_name.php';
require_once 'is_good_variants.php';
require_once 'is_good_urls.php';
require_once 'is_good_percs.php';
require_once 'inform_rts.php';

function chk_test(
  $test_id)
{
{
  //--- Logging 
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "insert_test_edit_test_basic");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;

  // START Check inputs
  assert(!empty($str_inJ));
  assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");
  $test_name = get_json_element($inJ, 'name'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  $test_dscr = get_json_element($inJ, 'description'); 
  $variants  = get_json_element($inJ, 'Variants');
  assert(is_array($variants));
  $nV = count($variants);
  assert($nV > 0 );
  //-----------------------------------------------
  //-- In subsequent versions, we will allow user to pick $bin_type
  //-- For now, following is hard coded
  switch ( $test_type ) {
  case "ABTest" :
    $bin_type =  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok";
    break;
  case "XYTest" :
    $bin_type = "free_for_all";
    break;
  default : 
    rs_assert(null, "Invalid test type $test_type");
    break;
  }
  $bin_type_id = lkp("bin_type", $bin_type);

  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= lkp("configs", "max_len_test_dscr"));
  }
  assert(is_good_test_name($test_name, $test_type));

  $test_type_id = lkp("test_type", $test_type);
  $draft_id     = lkp("state", "draft");

  $variant_ids   = array($nV);
  $variant_names = array($nV);
  $variant_percs = array($nV);
  $variant_urls  = array($nV);
  $vidx = 0;
  foreach ( $variants as $v ) { 
    if ( isset($v->{'id'}) ) {
      $variant_ids[$vidx] = $v->{'id'};
    }

    $name = $v->{'name'};
    assert(isset($name));
    assert(is_string($name));
    $variant_names[$vidx] = $name;

    $perc = $v->{'percentage'};
    assert(isset($perc));
    assert(is_string($perc));
    $perc = floatval($perc);
    $variant_percs[$vidx] = $perc;

    if ( isset($v->{'id'}) ) {
      $variant_urls[$vidx] = $v->{'url'};
    }

    $vidx++;
  }
  is_good_variants($variant_names, $bin_type);
  if ( $test_type == "XYTest" ) { 
    is_good_urls($variant_urls);
  }
  is_good_percs($variant_percs, $bin_type);
  // Now decide whether to update or insert 
  if ( ( isset($inJ->{'id'} )  && ($inJ->{'id'} == "" ) ) ||
    ( !isset($inJ->{'id'}) ) ) {
      $test_id = null;
      rs_assert(is_test_name_unique($test_name, $test_type),
        "test name [$test_name] not unique");
      $creator   = get_json_element($inJ, 'Creator');
      $creator_id   = lkp("admin", $creator);
    }
  else {
    $test_id = $inJ->{'id'};
    rs_assert(is_numeric($test_id));
    $test_id = intval($test_id);
    rs_assert(db_get_row("test", "id", $test_id));
    $updater    = get_json_element($inJ, 'Updater');
    $updater_id = lkp("admin", $updater);
  }
  // STOP Check inputs
  return true;
}
?>

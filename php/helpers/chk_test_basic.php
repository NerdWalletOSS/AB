<?php
// asserts out if error; else, Returns array of values 
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
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

function chk_test_basic(
  $inJ, // json representation of test 
  $dbg
)
{
  $test_name = get_json_element($inJ, 'name'); 
  if ( $dbg ) { echo("PREMATURE"); exit; }
  $test_type = get_json_element($inJ, 'TestType'); 
  $test_dscr = get_json_element($inJ, 'description'); 
  $variants  = get_json_element($inJ, 'Variants');
  $bin_type  = get_json_element($inJ, 'BinType');

  assert(is_array($variants));
  $nV = count($variants);
  rs_assert($nV >= lkp('configs', "min_num_variants"));
  rs_assert($nV <= lkp('configs', "max_num_variants"));
  //-----------------------------------------------
  switch ( $test_type ) {
  case "ABTest" :
    rs_assert($bin_type ==  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok");
    break;
  case "XYTest" :
    rs_assert($bin_type == "free_for_all");
    break;
  default : 
    rs_assert(null, "Invalid test type $test_type");
    break;
  }

  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= lkp("configs", "max_len_test_dscr"));
  }
  assert(is_good_test_name($test_name, $test_type)); // TODO THINK

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
  $chk_rslt['variant_names']  = $variant_names;
  $chk_rslt['variant_urls']   = $variant_urls;
  $chk_rslt['variant_percs']  = $variant_percs;
  $chk_rslt['variant_ids']    = $variant_ids;
  return $chk_rslt;
}
?>

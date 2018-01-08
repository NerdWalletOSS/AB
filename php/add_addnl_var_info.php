<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
/*
require_once 'is_test_name_unique.php';
require_once 'lkp.php';
require_once 'make_seed.php';
require_once 'get_time_usec.php';
 */
require_once 'aux.php';
require_once 'db_get_test.php';
require_once 'mod_row.php';
require_once 'db_get_variant.php';

function add_addnl_var_info(
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
  $test_name   = get_json($inJ, 'TestName'); 
  $test_type   = get_json($inJ, 'TestType'); 
  $variant     = get_json($inJ, 'Variant'); 
  $description = get_json($inJ, 'Description');
  $custom_data = get_json($inJ, 'CustomData');
  $url         = get_json($inJ, 'URL');

  $t = db_get_test(null, $test_name, $test_type);
  assert($t, "No test [$test_name] of type [$test_type]");
  $tid = $t['id'];

  $v = db_get_variant($tid, $variant);
  assert($v, "No variant [$variant] for test [$test_name] of type [$test_type]");
  $vid = $v['id'];

  mod_row("variant", "description", $description, "id = $vid");
  switch ( $test_type ) { 
  case "ABTest" :
    $custom_data_str = json_encode($custom_data);
    mod_row("variant", "custom_data", $custom_data_str, "id = $vid");
    break;
  case "XYTest" :
    mod_row("variant", "url", $url, "id = $vid");
    break;
  default : 
    assert(null, "Invalid test_type $test_type");
    break;
  }
  $outJ['status'] = "ok";
  return $outJ;
}
?>

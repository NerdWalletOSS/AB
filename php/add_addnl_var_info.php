<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'aux.php';
require_once 'db_get_test.php';
require_once 'mod_cell.php';
require_once 'db_get_variant.php';
require_once 'lkp.php';
require_once 'chk_url.php';
require_once 'aux_chk_name.php';

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

  $t = db_get_test(null, $test_name, $test_type);
  assert($t, "No test [$test_name] of type [$test_type]");
  $tid = $t['id'];

  $v = db_get_variant($tid, $variant);
  assert($v, "No variant [$variant] for test [$test_name] of type [$test_type]");
  $vid = $v['id'];

  mod_cell("variant", "description", $description, "id = $vid");
  switch ( $test_type ) { 
  case "ABTest" :
    $custom_data_str = json_encode($custom_data);
    mod_cell("variant", "custom_data", $custom_data_str, "id = $vid");
    break;
  case "XYTest" :
    break;
  default : 
    assert(null, "Invalid test_type $test_type");
    break;
  }
  $outJ['status'] = "ok";
  return $outJ;
}
?>

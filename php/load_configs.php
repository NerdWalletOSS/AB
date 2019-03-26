<?php  
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'make_pos_int.php';
require_once 'make_boolean.php';
require_once 'db_get_rows.php';
<<<<<<< HEAD
require_once 'extract_sql_params.php';

function read_config_file()
=======
function load_configs(
  $conf_file = "/opt/abadmin/db2.json",
  $reload=false
)
>>>>>>> dev
{
  if ( isset($GLOBALS['config_file_contents']) ) {
    return $GLOBALS['config_file_contents'];
  }

  $conf_file = dirname(__FILE__) . "/../config.json";

  //----------------------------------------
  if ( !is_file($conf_file) ) { 
    var_dump($conf_file);
    debug_print_backtrace(); return null;
  }
  $X = json_decode(file_get_contents($conf_file));
  if ( !$X ) { 
    debug_print_backtrace(); return null;
  }

  $GLOBALS['config_file_contents'] = $X;
  return $X;
}

function load_configs()
{
  // Exit early if already loaded 
  if ( isset($GLOBALS['configs']) ) {
    if ( $reload == false ) { 
      return true;
    }
  }

  $X = db_get_rows("config");
  rs_assert($X, "No table config in database");
  foreach ( $X as $x ) {
    $C[$x['name']] = $x['value'];
  }
  $C['check_url_reachable'] = make_boolean($C['check_url_reachable']);
  $C['num_retries'] = make_pos_int($C['num_retries']);
  $C['max_len_variant_dscr'] = make_pos_int($C['max_len_variant_dscr']);
  $C['max_len_admin_name'] = make_pos_int($C['max_len_admin_name']);
  $C['max_len_channel_name'] = make_pos_int($C['max_len_channel_name']);
  $C['max_len_cat_attr'] = make_pos_int($C['max_len_cat_attr']);
  $C['max_len_cat_attr_val'] = make_pos_int($C['max_len_cat_attr_val']);
  $C['max_len_regex'] = make_pos_int($C['max_len_regex']);
  $C['max_num_devices'] = make_pos_int($C['max_num_devices']);
  $C['max_num_tests'] = make_pos_int($C['max_num_tests']);
  $C['min_num_variants'] = make_pos_int($C['min_num_variants']);
  $C['max_num_variants'] = make_pos_int($C['max_num_variants']);
  $C['max_len_custom_data'] = make_pos_int($C['max_len_custom_data']);
  $C['max_len_test_name'] = make_pos_int($C['max_len_test_name']);
  $C['max_len_test_dscr'] = make_pos_int($C['max_len_test_dscr']);
  $C['max_len_variant_name'] = make_pos_int($C['max_len_variant_name']);
  $C['max_len_variant_dscr'] = make_pos_int($C['max_len_variant_dscr']);
  $C['max_len_url'] = make_pos_int($C['max_len_url']);
  if ( getenv("AB_WEBAPP_STANDALONE") == "true" ) {
    $C['rts_finder_server'] = null;
    $C['rts_finder_port']   = null;
  }
  $GLOBALS['configs'] = $C;
  return true;
}
?>

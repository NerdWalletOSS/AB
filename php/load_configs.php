<?php  
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'make_pos_int.php';
require_once 'make_boolean.php';
function load_configs(
  $conf_file = "/opt/abadmin/db.json",
  $reload=false
)
{
  // Exit early if already loaded 
  if ( isset($GLOBALS['configs']) ) {
    if ( $reload == false ) { 
      return true;
    }
  }
  // Over-ride default based on environment variable 
  $cf = getenv("AB_CONFIG_FILE");
  if ( ( $cf != "" ) && ( is_file($cf) ) )  {
    $conf_file = $cf;
  }
  //echo $conf_file;
  //----------------------------------------
  if ( !is_file($conf_file) ) { 
    debug_print_backtrace(); return null;
  }
  $X = json_decode(file_get_contents($conf_file));
  if ( !$X ) { 
    debug_print_backtrace(); return null;
  }
  //----------------------------------------
  $dbhost = $X->{"dbhost"};
  $dbname = $X->{"dbname"};
  $dbuser = $X->{"dbuser"};
  $dbpass = $X->{"dbpass"};
  //----------------------------------------
  $rts_finder_server = "";
  $rts_finder_port   = "";
  if ( isset($X->{"rts_finder_server"}) ) {
    $rts_finder_server = trim($X->{"rts_finder_server"});
    if ( $rts_finder_server != "" ) { 
      if ( !isset($X->{"rts_finder_port"}) ) {
        debug_print_backtrace(); return null; 
      }
      $rts_finder_port = $X->{"rts_finder_port"};
      if ( ( is_null($rts_finder_port) ) || 
        ( $rts_finder_port == "" ) || 
        ( !is_numeric($rts_finder_port) ) || 
        ( $rts_finder_port <= 0 ) ) {
          debug_print_backtrace(); return null;
        }
    }
    if ( !isset($X->{"ab_rts_port"}) ) {
      debug_print_backtrace(); return null;
    }
  }
  //----------------------------------------
  $server = "";
  $port   = "";
  if ( isset($X->{"ab_rts_server"}) ) {
    $server = $X->{"ab_rts_server"};
  }
  //-------------------------------------------------
  if ( ( $server != "" ) && ( $rts_finder_server != "" ) ) { 
    // echo "Cannot set both ab_rts_server and rts_finder_server\n";
    // debug_print_backtrace(); return null;
    // We will not treat this as error for now but will use RTS Finder
  }
  //-------------------------------------------------------
  if ( ( $server == "" ) && ( $rts_finder_server == "" ) ) { 
    // Means we are just testing Webapp and no need for port in this case 
    $port = "";
  }
  else { // port must be defined 
    if ( !isset($X->{"ab_rts_port"}) ) {
      debug_print_backtrace(); return null;
    }
    $port = $X->{"ab_rts_port"};
    if ( ( is_null($port) ) || ( $port == "" ) || 
      ( !is_numeric($port) ) || ( $port <= 0 ) ) {
        debug_print_backtrace(); return null;
      }
  }
  //----------------------------------------
  $C['dbhost'] = $dbhost; 
  $C['dbname'] = $dbname; 
  $C['dbuser'] = $dbuser; 
  $C['dbpass'] = $dbpass; 
  $C['server'] = $server; 
  $C['port']   = $port; 
  //----------------------------------------
  unset($X);
  $X = db_get_rows("config");
  assert(isset($X));
  foreach ( $X as $x ) {
    $C[$x['name']] = $x['value'];
  }
  foreach ( $C as $c ) {
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
  }
  $GLOBALS['configs'] = $C;
  return true;
}
?>

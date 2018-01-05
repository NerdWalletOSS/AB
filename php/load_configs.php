<?php  
function load_configs(
  $conf_file = "/opt/abadmin/db.json",
  $reload=false
)
{
  // Exit early if already loaded 
  if ( isset($GLOBALS['CONFIGS']) ) {
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
  $check_url_reachable = $X->{"check_url_reachable"};
  if ( $check_url_reachable != "" ) { 
    if ( ( $check_url_reachable != "true" ) && ( $check_url_reachable != "false" ) ) {
      debug_print_backtrace(); return null;
    }
  }
  //----------------------------------------
  $webapp_server = "";
  $webapp_port   = "";
  if( isset( $X->{"webapp_server"}) ) { 
    $webapp_server = $X->{"webapp_server"};
    if ( $webapp_server != "" ) { 
      $webapp_port = $X->{"webapp_port"};
      if ( ( is_null($webapp_port) ) || ( $webapp_port == "" ) || 
        ( !is_numeric($webapp_port) ) || ( $webapp_port <= 0 ) ) {
          debug_print_backtrace(); return null;
        }
    }
  }
  //----------------------------------------
  $log_server = "";
  $log_port   = "";
  if ( isset($X->{"ab_log_server"}) ) { 
    $log_server = trim($X->{"ab_log_server"});
    if ( $log_server != "" ) { 
      $log_port = trim($X->{"ab_log_port"});
      if ( ( is_null($log_port) ) || ( $log_port == "" ) || 
        ( !is_numeric($log_port) ) || ( $log_port <= 0 ) ) {
          debug_print_backtrace(); return null;
        }
    }
  }
  //----------------------------------------
  if ( ( $port > 0 ) && ( $log_port > 0 ) )  {
    if ( $port == $log_port ) { debug_print_backtrace(); return null; }
  }
  if ( ( $port > 0 ) && ( $webapp_port > 0 ) )  {
    if ( $port == $webapp_port ) { debug_print_backtrace(); return null; }
  }
  //----------------------------------------
  $default_landing_page = $X->{"default_landing_page"};
  $log_requests = "";
  if ( isset($X->{"log_requests"}) ) { 
    $log_requests = $X->{"log_requests"};
  }
  //----------------------------------------
  $GLOBALS['CONFIGS'] = array(
    'dbhost' => $dbhost, 
    'dbname' => $dbname, 
    'dbuser' => $dbuser, 
    'dbpass' => $dbpass, 
    'num_retries' => 10,
    'log_server' => $log_server, 
    'log_port' => $log_port,
    'webapp_server' => $webapp_server, 
    'webapp_port' => $webapp_port,
    'rts_finder_server' => $rts_finder_server, 
    'rts_finder_port' => $rts_finder_port,
    'server' => $server, 
    'port' => $port,
    'check_url_reachable' => $check_url_reachable,
    'log_requests' => $log_requests,
    'default_landing_page' => $default_landing_page,
    'max_len_admin_name' => 31,
    'max_len_channel_name' => 15,
    'max_len_cat_attr' => 15,
    'max_len_cat_attr_val' => 15,
    'max_len_regex' => 1023, // HARD CODED
    'max_num_devices' => 8, // HARD CODED 
    'max_num_tests' => 1024, // HARD CODED 
    'min_num_variants' => 2, // HARD CODED
    'max_num_variants' => 8, // HARD CODED
    'max_len_custom_data' => 2047, // HARD CODED
    'max_len_test_name' => 127, // HARD CODED
    'max_len_test_dscr' => 255, // HARD CODED. Not used by RTS
    'max_len_variant_name' => 31, // HARD CODED
    'max_len_variant_dscr' => 255 // HARD CODED
  );
  return true;
}
?>

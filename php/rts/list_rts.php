<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "get_url.php";

function list_rts(
)
{
  $servers = null;
  $ports   = null;
  // Load configs 
  rs_assert(load_configs());
  $conf_file = "/opt/abadmin/db2.json";
  rs_assert(is_file($conf_file), "File not found $conf_file");
  $configs = json_decode(file_get_contents($conf_file));
  rs_assert($configs, "unable to JSON decode $conf_file");
  //-----------------------------------------------------------
  //--- Original case: just one server
  if ( $configs->{'rts_finder_server'} == "" ) {
    $server = $configs->{'ab_rts_server'};
    $port   =  $configs->{'ab_rts_port'};
    if ( ( $server == "" ) || ($port == "" ) )  {
      // echo("NO RTS found\n");
      return null;
    }

    $SP = array(1);
    $SP[0] = array('server' => $server, 'port' => $port);
    if ( getenv("AB_WEBAPP_STANDALONE") == "true" ) {
      $SP = null;
    }
    return $SP;
  }
  //-----------------------------------------------------------
  if ( ( $configs->{'rts_finder_server'} != "" ) && 
       ( $configs->{'rts_finder_port'} != "" ) )  {
    // do something 
    $s = $configs->{'rts_finder_server'} ;
    $p = $configs->{'rts_finder_port'} ;
    $http_code = 0; $rslt = "";
    get_url($s, $p, "DescribeInstances", $http_code, $rslt);
    if ( $http_code != 200 ) { return null; }
    $X = json_decode($rslt);
    if ( !$X ) { return null; }
    if ( !isset($X->{'server'}) ) { return null; } if ( !isset($X->{'port'}) ) { return null; }
    $servers = $X->{'server'};
    $ports = $X->{'port'};
    if ( count($servers) != count($ports) )  { return null; }
    $n = count($servers);
    $SP = array($n);
    for ( $i = 0; $i < $n; $i++ ) { 
      $SP[$i] = array('server' => $servers[$i], 'port' => $ports[$i]);
    }
    return $SP;
  }
  else {
    // echo("2: NO RTS found\n");
    return null;
  }
}
/*
$SP = list_rts();
if ( $SP ) { 
   foreach ( $SP as $sp ) { 
     $server = $sp['server']; $port = $sp['port'];
     echo "server = $server, port = $port \n";
  }
}
else {
  echo "Not using RTS\n";
}
*/
?>

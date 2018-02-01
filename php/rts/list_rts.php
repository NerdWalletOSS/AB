<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "post_url.php";

function list_rts(
)
{
  return null;
  // TODO P0
  $servers = null;
  $ports   = null;
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return null;
  }
  //-----------------------------------------------------------
  $configs = $GLOBALS['CONFIGS'];
  if ( ( !isset($configs['port']) ) ||  ( $configs['port'] == "" ) ) { 
    return null;
  }

  //--- Original case: just one server
  if ( $configs['rts_finder_server'] == "" ) {
    $server = $configs['server'];
    $port   =  $configs['port'];

    $SP = array(1);
    $SP[0] = array('server' => $server, 'port' => $port);
    return $SP;
  }
  //-----------------------------------------------------------
  if ( ( $configs['rts_finder_server'] != "" ) && 
    ( $configs['rts_finder_port'] != "" ) )  {
      // do something 
      $s = $configs['rts_finder_server'] ;
      $p = $configs['rts_finder_port'] ;
      $http_code = 0; $rslt = "";
      post_url($s, $p, "DescribeInstances", $http_code, $rslt);
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
        echo "$n:$i:{$servers[$i]}:{$ports[$i]}\n";
        trigger_error("$n:$i:{$servers[$i]}:{$ports[$i]}\n");
      }
      return $SP;
    }
  else {
    echo("NO RTS found\n");
    return null;
  }
}
/*
$SP = list_rts($servers, $ports);
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

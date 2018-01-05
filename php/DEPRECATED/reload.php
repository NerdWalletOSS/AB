<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
require_once "bye.php";
require_once "yurl.php";

function reload(
  $rts_host,
  $rts_port
)
{
  $http_code = 0; $rslt = "";
  yurl($rts_host, $rts_port, "Ignore", $http_code, $rslt);
  if ( $http_code != 200 ) { 
    $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $http_code = 0; $rslt = "";
  yurl($rts_host, $rts_port, "GenDBConfigs", $http_code, $rslt);
  if ( $http_code != 200 ) { 
    $GLOBALS['err'] .= "RTS error:   $rslt\n";
    $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $X = json_decode($rslt);  
  if ( !$X ) { 
    $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  for ( $done = false; $done == false ; ) {
    yurl($rts_host, $rts_port, "ReloadFromDBConfigs", $http_code, $rslt);
    if ( $http_code != 200 ) { 
      $GLOBALS['err'] .= "RTS error:   $rslt\n";
      $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $X = json_decode($rslt);  
    if ( !$X ) { 
      $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    if ( !isset($X->{"ReloadFromDBConfigs"} ) ) {
      $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $status = $X->{"ReloadFromDBConfigs"};
    switch ( $status ) { 
    case "OK" : 
      $done = true;
      break;
    case "TRY_AGAIN" : 
      sleep(1); 
      break;
    default : 
      $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
  }
  return true;
}

$GLOBALS['err'] = "";
if ( !isset($_GET['rts_host']) ) { 
  $GLOBALS['err'] = "RTS Host not set \n";
  $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  goto ERR;
}
$rts_host = $_GET['rts_host']; 
if ( $rts_host == "" ) { 
  $GLOBALS['err'] = "RTS Host not valid [$rts_host]\n";
  $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  goto ERR; 
}
//------------------------------------------------
if ( !isset($_GET['rts_port']) ) { 
  $GLOBALS['err'] = "RTS Port not set \n";
  $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  goto ERR; 
}
$rts_port = $_GET['rts_port']; 
if ( $rts_port != "" ) {
  if ( ( !is_numeric($rts_port))  || (  $rts_port <= 0 ) )  { 
    $GLOBALS['err'] = "RTS Port not valid [$rts_port]\n";
    $GLOBALS['err'] = "ERR: FILE " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    goto ERR; 
  }
}
$rslt = reload($rts_host, $rts_port);
if ( !$rslt ) { goto ERR; }
header("HTTP/1.1 200 OK");
exit;
ERR: 
header("HTTP/1.1 500 Internal Server Error");
$x = str_replace("\n", "<br>", $GLOBALS['err']);
echo $x
?>

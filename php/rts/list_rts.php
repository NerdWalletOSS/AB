<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "get_url.php";

function list_rts()
{
  $config = read_config_file();

  switch ($config->{'AB'}->{'RTS_FINDER'}->{'METHOD'}->{'VALUE'}) {
  case "ecs":
    break;

  case "local":
    $ret = array();
    foreach ($config->{'AB'}->{'RTS_FINDER'}->{'SERVERS'}->{'VALUE'} as $server) {
      $ret[] = (array) $server;
    }
    return $ret;

  default:
    echo("Unknown RTS finder method");
  }
}

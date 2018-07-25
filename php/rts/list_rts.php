<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "get_url.php";
require_once 'aws.phar';

use Aws\Ecs\EcsClient;

function list_rts()
{
  $config = read_config_file();

  switch ($config->{'AB'}->{'RTS_FINDER'}->{'METHOD'}->{'VALUE'}) {
  case "ecs":
    $client = EcsClient::factory(array(
      'profile' => 'nwdev',
      'region'  => 'us-east-1',
      'version' => '2014-11-13',
    ));
    $result = $client->listTasks([
      'desiredStatus' => 'RUNNING',
      'cluster' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_CLUSTER'}->{'VALUE'},
      'family' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_FAMILY'}->{'VALUE'},
    ]);

    $result = $client->describeTasks([
      'cluster' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_CLUSTER'}->{'VALUE'},
      'tasks' => $result['taskArns'],
    ]);

    $ret = array();
    foreach ($result['tasks'] as $task) {
      foreach ($task['containers'] as $container) {
        $ret[] = array(
          'server' => $container['networkBindings'][0]['bindIP'],
          'port' => $container['networkBindings'][0]['hostPort'],
        );
      }
    }
    return $ret;

  case "local":
    // our config comes from loaded JSON, which is all stdObject
    // the code that consumes it is built to work with arrays, so this transforms our objects into arrays
    $ret = array();
    foreach ($config->{'AB'}->{'RTS_FINDER'}->{'SERVERS'}->{'VALUE'} as $server) {
      $ret[] = (array) $server;
    }
    return $ret;

  default:
    echo("Unknown RTS finder method");
    die;
  }
}

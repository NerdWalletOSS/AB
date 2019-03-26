<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "get_url.php";
require_once 'aws.phar';

use Aws\Ec2\Ec2Client;
use Aws\Ecs\EcsClient;

function list_rts()
{
<<<<<<< HEAD
  $config = read_config_file();

  switch ($config->{'AB'}->{'RTS_FINDER'}->{'METHOD'}->{'VALUE'}) {
  case "ecs":
    // Create our clients
    $ecsClient = EcsClient::factory(array(
      'region'  => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_REGION'}->{'VALUE'},
      'version' => '2014-11-13',
    ));
    $ec2Client = new Ec2Client([
      'region'  => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_REGION'}->{'VALUE'},
      'version' => '2016-11-15',
    ]);

    // Get a list of running tasks
    $result = $ecsClient->listTasks([
      'desiredStatus' => 'RUNNING',
      'cluster' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_CLUSTER'}->{'VALUE'},
      'family' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_FAMILY'}->{'VALUE'},
    ]);

    // Describe those tasks
    $result = $ecsClient->describeTasks([
      'cluster' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_CLUSTER'}->{'VALUE'},
      'tasks' => $result['taskArns'],
    ]);
=======
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
>>>>>>> dev

    // Extract each container as an instance of our RTS
    $ret = array();
    foreach ($result['tasks'] as $task) {
      // Lookup the container instance for this task
      $containerInstance = $ecsClient->describeContainerInstances([
        'cluster' => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_CLUSTER'}->{'VALUE'},
        'containerInstances' => [$task['containerInstanceArn']],
      ]);

      // Describe the EC2 instance so we can get the private IP address
      $ec2Instance = $ec2Client->describeInstances([
        'InstanceIds' => [$containerInstance['containerInstances'][0]['ec2InstanceId']]
      ]);
      $hostIp = $ec2Instance['Reservations'][0]['Instances'][0]['PrivateIpAddress'];

      // For each container add it as a server running on that port
      foreach ($task['containers'] as $container) {
        $ret[] = array(
          'server' => $hostIp,
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
<<<<<<< HEAD
    return $ret;

  default:
    echo("Unknown RTS finder method");
    die;
  }
}
=======
    return $SP;
  }
  else {
    // echo("NO RTS found\n");

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
>>>>>>> dev

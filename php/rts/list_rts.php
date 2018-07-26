<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "load_configs.php";
require_once "get_url.php";
require_once 'aws.phar';

use Aws\Credentials\CredentialProvider;
use Aws\Ec2\Ec2Client;
use Aws\Ecs\EcsClient;

function list_rts()
{
  $config = read_config_file();

  switch ($config->{'AB'}->{'RTS_FINDER'}->{'METHOD'}->{'VALUE'}) {
  case "ecs":
    // We must construct an explicit credential chain that contains the ECS provider
    // The default provider only does env, ini & instanceProfile.  See:
    // https://docs.aws.amazon.com/sdk-for-php/v3/developer-guide/guide_credentials_provider.html#chaining-providers
    $credentials = CredentialProvider::memoize(
      CredentialProvider::chain(
        CredentialProvider::env(),
        CredentialProvider::ini(),
        CredentialProvider::ecsCredentials(),
        CredentialProvider::instanceProfile([])
      )
    );

    // Create our clients
    $ecsClient = EcsClient::factory(array(
      'region'  => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_REGION'}->{'VALUE'},
      'version' => '2014-11-13',
      'credentials' => $credentials,
    ));
    $ec2Client = new Ec2Client([
      'region'  => $config->{'AB'}->{'RTS_FINDER'}->{'ECS_REGION'}->{'VALUE'},
      'version' => '2016-11-15',
      'credentials' => $credentials,
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

      // For each container 
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
    return $ret;

  default:
    echo("Unknown RTS finder method");
    die;
  }
}

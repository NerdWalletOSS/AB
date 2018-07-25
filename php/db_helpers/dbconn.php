<?php
require_once 'bye.php';
require_once 'load_configs.php';

function dbconn()
{
  if ( isset($_GLOBALS['DBH']) ) {
    return $GLOBALS['DBH'];
  }

  $X = read_config_file();
  $dbuser = $X->{"AB"}->{"MYSQL"}->{'USER'}->{'VALUE'};
  $dbpass = $X->{"AB"}->{"MYSQL"}->{'PASSWORD'}->{'VALUE'};
  $dbname = $X->{"AB"}->{"MYSQL"}->{'DATABASE'}->{'VALUE'};
  $dbhost = $X->{"AB"}->{"MYSQL"}->{'SERVER'}->{'VALUE'};
  $dbport = $X->{"AB"}->{"MYSQL"}->{'PORT'}->{'VALUE'};

  $dbh = new PDO("mysql:host=$dbhost;dbname=$dbname;port=$dbport;charset=utf8", $dbuser, $dbpass);
  if ( !$dbh ) { go_BYE(""); }
  $GLOBALS['DBH'] = $dbh;
  return $dbh;  
}

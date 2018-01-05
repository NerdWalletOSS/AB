<?php
function get_time_usec()
{
  $t_original = microtime();
  $t_sec = preg_replace('/^.*[ ]/', '', $t_original);
  $t_mu_sec = preg_replace('/ .*$/', '', $t_original);
  $t_mu_sec = str_replace('0.', '', $t_mu_sec);
  $t_final =  $t_sec * 100000000 + $t_mu_sec;
  return $t_final;
}
function get_date()
{
  date_default_timezone_set('America/Los_Angeles');
  $dateTime = new DateTime("now");
  $dateTime->setTimeZone(new DateTimeZone('America/Los_Angeles'));
  return $dateTime->format('Y-m-d H:i:s');
}
?>

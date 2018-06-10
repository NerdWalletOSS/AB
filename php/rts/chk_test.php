<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "get_url.php";
require_once "get_json_element.php";
require_once "list_rts.php";
require_once "db_get_test.php";


function chk_test(
  $test_name,
  $test_type
)
{
  $rslt = "";
  $http_code = 0;

  $SP = list_rts(); 
  rs_assert($SP, "No RTS listening");
  foreach ($SP as $sp ) { 
    $S = $sp['server'];
    $P = $sp['port'];
    $url  = "TestInfo?Source=C&TestName=$test_name&TestType=$test_type";
    get_url($S, $P, $url, $http_code, $rslt);
    rs_assert($http_code == 200, "Bad http code from RTS");
    $T1 = json_decode($rslt);
    rs_assert($T1, "bad JSON from RTS");
    $T2 = db_get_test(null, $test_name, $test_type);
    rs_assert($T2, "test not found");
    //-- START:  error checking UTPAL TODO P0
    rs_assert($T1->{'State'} == $T2['State']);
    //-- STOP :  error checking
  }
  return true;
}
$x = chk_test("Test1", "XYTest");
var_dump($x);
?>

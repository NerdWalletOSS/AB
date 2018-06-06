<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
require_once "rs_assert.php";
require_once "get_url.php";
require_once "get_json_element.php";
require_once "list_rts.php";

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
    $J = json_decode($rslt);
    rs_assert($J, "bad JSON from RTS");
    print(get_json_element($J, "name"));
  }
}
chk_test("Test_1", "XYTest");
?>

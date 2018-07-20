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
  $str_inJ
)
{
  $rslt = "";
  $http_code = 0;

  $inJ = json_decode($str_inJ);
  rs_assert($inJ);
  $test_name = get_json_element($inJ, 'TestName');
  $test_type = get_json_element($inJ, 'TestType');

  $SP = list_rts();  
  rs_assert($SP, "No RTS listening"); 
  foreach ($SP as $sp ) { 
    $S = $sp['server']; 
    $P = $sp['port']; 
    $url  = "TestInfo?Source=C&TestName=$test_name&TestType=$test_type";
    get_url($S, $P, $url, $http_code, $rslt, $destination); 
    rs_assert($http_code == 200, "Bad http code from RTS"); 
    $T1 = json_decode($rslt);
    var_dump($rslt);
    rs_assert($T1, "bad JSON from RTS");
    $T2 = db_get_test(null, $test_name, $test_type);
    //print("<pre>".print_r($T2,true)."</pre>");
    rs_assert($T2, "test not found");
    //-- START:  error checking UTPAL TODO P0
    rs_assert($T1->{'State'} == $T2['State']);
    rs_assert($T1->{'id'} == $T2['id']);
    if ($test_type == "XYTest") {
    rs_assert($T1->{'is_dev_specific'} == $T2['is_dev_specific']);
    }
    if ($test_type == "ABTest") {
    rs_assert($T1->{'has_filters'} == $T2['has_filters']);
    }
    
    
    //-- STOP :  error checking
  }
  return true;
}
$x = chk_test(' { "TestName" : "T3", "TestType" : "ABTest" } ');
var_dump($x);

?>

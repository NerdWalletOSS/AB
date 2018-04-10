<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'db_get_test.php';
require_once 'db_get_rows.php';
require_once 'get_json_element.php';
require_once 'lkp.php';
//-----------------------------------------------------------
if ( ( !empty($_GET) ) && ( isset($_GET['TestType']) ) ) {
  $l_test_type= $_GET['TestType'];
}
else {
  $body = file_get_contents('php://input');
  if ( ( empty($body) ) || ( trim($body) == "" ) ) {
    echo '{ "ListTests" : "ERROR", "Message" : "No payload" }'; exit;
  }
  $X = json_decode($body); 
  if ( !$X ) { 
    echo '{ "ListTests" : "ERROR", "Message" : "Invalid JSON" }'; exit;
  }
  $l_test_type = get_json_element($X, "TestType");
}
$started_id    = lkp("state", "started");
$terminated_id = lkp("state", "terminated");
$abtest_id = lkp("test_type", "ABTest");
$xytest_id = lkp("test_type", "XYTest");
$where = " ( state_id = $started_id or state_id = $terminated_id ) ";
if ( $l_test_type == "ABTest" ) {
  $where .= " and test_type_id = $abtest_id ";
}
else if ( $l_test_type == "XYTest" ) {
  $where .= " and test_type_id = $xytest_id ";
}
else if ( $l_test_type == "" ) {
  $where .= " and test_type_id = $xytest_id ";
}
else {
  echo "{ \"ListTests\" : \"ERROR\", \"Message\" : \"Invalid TestType=[$l_test_type]\" }"; exit;
}
$R = db_get_rows("test", $where);
if ( empty($R) ) { 
  echo "[]";
}
else { 
  $i = 0;
  foreach ( $R as $r ) { 
    $R[$i]['State'] = lkp("state", $r['state_id'], "reverse");
    $i++;
  }
  echo json_encode($R, JSON_PRETTY_PRINT);
}
?>

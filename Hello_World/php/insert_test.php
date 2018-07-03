<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
require_once 'rs_assert.php';

function insert_test(
  $str_inJ
)
{
  $outJ['status'] = "error";
  $outJ['stdout'] = "";
  $outJ['stderr'] = "";
  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); rs_assert($inJ, "invalid JSON");
	$test_name = $inJ ->TestName; 
	$test_dscr = $inJ ->TestDescription; 
  //$test_name = get_json($inJ, 'TestName'); 
  //$test_dscr = get_json($inJ, 'TestDescription'); 

  if ( isset($test_name) ) {
    rs_assert(is_string($test_name));
    rs_assert(!empty($test_name), "Please enter Test Name");
		rs_assert(strlen($test_name) <=5, "Test Name too long");
  }

  if ( isset($test_dscr) ) {
    rs_assert(is_string($test_dscr));
    rs_assert(!empty($test_dscr), "Please enter Test Description");
		rs_assert(strlen($test_dscr) <=10, "Test Description too long");
  }
  
  //------------------------------------------
  $outJ["status"] = "ok";
  $outJ["stdout"] = "Created test $test_name";
  $outJ["test_id"] = $test_id;
  return $outJ;
}

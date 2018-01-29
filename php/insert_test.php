<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'dbconn.php';
require_once 'insert_row.php'; // NO PROBLEM
require_once 'make_seed.php'; // NO PROBLEM
require_once 'get_time_usec.php'; // NO PROBLEM
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'is_good_test_name.php';
require_once 'aux_chk_name.php';
require_once 'is_good_variants.php';
require_once 'is_good_urls.php';
require_once 'is_good_percs.php';

function insert_test(
  $str_inJ
)
{
  $d_create =  $d_update = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  //--- Logging 
  $txn_type_id   = lkp("txn_type", "insert_test_edit_test_basic");
  $X0['d_create'] = $d_create;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = lkp("api", "insert_test_edit_test_basic");
  $log_id = insert_row("request_webapp", $X0);
  $_SESSION['LOG_ID'] = $log_id;

  // START Check inputs
  assert(!empty($str_inJ));
  assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");
  $test_name = get_json_element($inJ, 'TestName'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  $test_dscr = get_json_element($inJ, 'TestDescription'); 
  $creator   = get_json_element($inJ, 'Creator');
  $variants  = get_json_element($inJ, 'Variants');
  assert(is_array($variants));
  $nV = count($variants);
  assert($nV > 0 );

  if ( isset($test_dscr) ) {
    assert(is_string($test_dscr));
    assert(strlen($test_dscr) <= lkp("configs", "max_len_test_dscr"));
  }
  assert(is_good_test_name($test_name, $test_type));

  $test_type_id = lkp("test_type", $test_type);
  $creator_id   = lkp("admin", $creator);
  $draft_id     = lkp("state", "draft");

  $variant_names = array($nV);
  $variant_percs = array($nV);
  $variant_urls  = array($nV);
  $vidx = 0;
  foreach ( $variants as $v ) { 
    $name = $v->{'Name'};
    assert(isset($name));
    assert(is_string($name));

    $perc = $v->{'Percentage'};
    assert(isset($perc));
    assert(is_string($perc));
    $perc = floatval($perc);
    
    $url = $v->{'URL'};
    assert(isset($url));
    assert(is_string($url));

    $variant_names[$vidx] = $name;
    $variant_percs[$vidx] = $perc;
    $variant_urls[$vidx]   = $url;

    $vidx++;
  }
  assert(is_good_variants($variant_names));
  assert(is_good_urls($variant_urls));
  assert(is_good_percs($variant_percs));
  // Now decide whether to update or insert 
  /*
  if ( isset($inJ->{'TestID'} ) { 
    $test_id = $inJ->{'TestID'};
  }
   */
  // STOP Check inputs
  //----------------------------------------------------
  $test_id = -1;
  $X1['name']         = $test_name;
  $X1['txn_type_id']     = $txn_type_id;
  $X1['request_webapp_id']  = $log_id;
  $X1['description']  = $test_dscr;
  $X1['test_type_id'] = $test_type_id;
  $X1['seed']         = make_seed();
  $X1['external_id']  = $t_create;
  $X1['d_create']     = $d_create;
  $X1['t_create']     = $t_create;
  $X1['d_update']     = $d_update;
  $X1['t_update']     = $t_update;
  $X1['creator_id']   = $creator_id;
  $X1['updater_id']   = $creator_id;
  $X1['state_id']     = $draft_id;
  //-----------------------------------------------
  //-- In subsequent versions, we will allow user to pick $bin_type
  //-- For now, following is hard coded
  switch ( $test_type ) {
  case "ABTest" :
    $bin_type_id = lkp("bin_type", "c_to_v_ok_v_to_c_ok_v_to_v_not_ok");
    break;
  case "XYTest" :
    $bin_type_id = lkp("bin_type", "free_for_all");
    break;
  default : 
    assert(null, "Invalid test type $test_type");
    break;
  }
  assert(isset($bin_type_id));
  $X1['bin_type_id'] = $bin_type_id;
  //-----------------------------------------------
  $dbh = dbconn(); assert(!empty($dbh)); 
  try {
    $dbh->beginTransaction();
    $test_id = insert_row("test", $X1);

    $X2['request_webapp_id']     = $log_id;
    $X2['txn_type_id']  = $txn_type_id;
    $X2['test_id']  = $test_id;
    $X2['t_update'] = $t_update;
    $X2['d_update'] = $d_update;
    //-------------------------------------------------------
    for ( $i = 0; $i < $nV; $i++ ) { 
      $X2['percentage']  = $variant_percs[$i];
      $X2['name']        = $variant_names[$i];
      insert_row("variant", $X2);
    }
    //------------------------------------------
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //------------------------------------------
  $outJ["status"] = "ok";
  $outJ["stdout"] = "Created test $test_name";
  $outJ["test_id"] = $test_id;

  unset($Y);
  $Y['msg_stdout'] = $outJ["stdout"];
  $Y['status_code'] = 200;
  db_set_row("log_ui_to_webapp", $log_id, $Y);

  header("Error-Code: 200");
  http_response_code(200);
  return $outJ;
}

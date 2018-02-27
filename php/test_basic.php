<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once 'dbconn.php';
require_once 'insert_row.php'; // NO PROBLEM
require_once 'make_seed.php'; // NO PROBLEM
require_once 'get_time_usec.php'; // NO PROBLEM
require_once 'lkp.php';
require_once 'get_json_element.php';
require_once 'db_get_row.php';
require_once 'mod_row.php';
require_once 'chk_test_basic.php';
require_once 'inform_rts.php';
require_once 'is_new_test.php';

function test_basic(
  $str_inJ
)
{
  //--- Logging 
  $created_at =  $updated_at = get_date(); 
  $t_create =  $t_update = get_time_usec(); 
  $api_id   = lkp("api", "insert_test_edit_test_basic");
  $X0['created_at'] = $created_at;
  $X0['t_create'] = $t_create;
  $X0['payload']  = $str_inJ;
  $X0['api_id']   = $api_id;
  $request_webapp_id = insert_row("request_webapp", $X0);
  $_SESSION['REQUEST_WEBAPP_ID'] = $request_webapp_id;

  // START Check inputs
  assert(!empty($str_inJ));
  assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");
  $test_name = get_json_element($inJ, 'name'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  //-----------------------------------------------
  // Decide whether to update or insert 
  $is_new = is_new_test($inJ);
  if ( $is_new ) { // if insert
    $test_id = null;
    rs_assert(is_test_name_unique($test_name, $test_type),
      "test name [$test_name] not unique");
    $creator   = get_json_element($inJ, 'Creator');
    $creator_id   = lkp("admin", $creator);
  }
  else { // if update
    $test_id = $inJ->{'id'};
    rs_assert(is_numeric($test_id));
    $test_id = intval($test_id);
    rs_assert(db_get_row("test", "id", $test_id));
    $updater    = get_json_element($inJ, 'Updater');
    $updater_id = lkp("admin", $updater);
  }
  if ( $is_new ) {
    switch ( $test_type ) {
    case "ABTest" :
      $bin_type =  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok";
      break;
    case "XYTest" :
      $bin_type = "free_for_all";
      break;
    default : 
      rs_assert(null, "Invalid test type $test_type");
      break;
    }
    $inJ->{'State'}   = $state = "draft";
    $inJ->{'BinType'} = $bin_type;
  }

  $state     = get_json_element($inJ, 'State');
  $test_name = get_json_element($inJ, 'name'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  $test_dscr = get_json_element($inJ, 'description'); 
  $variants  = get_json_element($inJ, 'Variants');
  $bin_type  = get_json_element($inJ, 'BinType');
  $state     = get_json_element($inJ, 'State');
  //-------------------------------------------------
  $test_type_id = lkp("test_type", $test_type);
  $bin_type_id  = lkp("bin_type", $bin_type);
  $state_id     = lkp("state", $state);

  $chk_rslt = chk_test_basic($inJ); 
  rs_assert($chk_rslt);
  $variant_percs = $chk_rslt['variant_percs'];
  $variant_ids   = $chk_rslt['variant_ids'];
  $variant_names = $chk_rslt['variant_names'];
  $variant_urls  = $chk_rslt['variant_urls'];
  // STOP Check inputs
  //----------------------------------------------------
  $X1['request_webapp_id']  = $request_webapp_id;
  $X1['api_id']       = $api_id;
  $X2['request_webapp_id']  = $request_webapp_id;
  $X2['api_id']       = $api_id;
  if ( $test_id > 0 ) {  // update
    $action = "updated";
    $state = get_json_element($inJ, 'State');
    rs_assert($state != "archived");  // no changes to archived state
    $X1['description']  = $test_dscr;
    $X1['updated_at']   = $updated_at;
    $X1['t_update']     = $t_update;
    $X1['updater_id']   = $updater_id;
    if ( $state == "draft" ) { 
      $X1['name']  = $test_name;
    }
    //-----------------------------------------------
    $dbh = dbconn(); assert(isset($dbh)); 
    try {
      $dbh->beginTransaction();
      mod_row("test", $X1, "where id = $test_id ");
      //-------------------------------------------------------
      $X2['t_update'] = $t_update;
      $X2['updated_at'] = $updated_at;
      for ( $i = 0; $i < count($variants); $i++ ) {
        if ( ( $state == "draft" ) || ( $state == "dormant" ) ) { 
          $X2['name']        = $variant_names[$i];
        }
        if ( ( $state == "draft" ) || ( $state == "dormant" ) ||
          ( $state == "started" ) ) {
            $X2['percentage']  = $variant_percs[$i];
            if ( $test_type == "XYTest" ) {
              $X2['url']        = $variant_urls[$i];
            }
          }
        mod_row("variant", $X2, "where id = " . $variant_ids[$i]);
      }
      //------------------------------------------
      $dbh->commit();
    } catch ( PDOException $ex ) {
      $dbh->rollBack();
      $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
  } 
  else { // insert
    $action = "inserted";
    $state = "draft";
    $X1['name']         = $test_name;
    $X1['description']  = $test_dscr;
    $X1['test_type_id'] = $test_type_id;
    $X1['seed']         = make_seed();
    $X1['external_id']  = $t_create;
    $X1['created_at']     = $created_at;
    $X1['t_create']     = $t_create;
    $X1['updated_at']     = $updated_at;
    $X1['t_update']     = $t_update;
    $X1['creator_id']   = $creator_id;
    $X1['updater_id']   = $creator_id;
    $X1['state_id']     = $state_id;
    $X1['bin_type_id'] = $bin_type_id;
    //-----------------------------------------------
    $dbh = dbconn(); assert(!empty($dbh)); 
    try {
      $dbh->beginTransaction();
      $test_id = insert_row("test", $X1);

      $X2['test_id']  = $test_id;
      $X2['t_update'] = $t_update;
      $X2['updated_at'] = $updated_at;
      //-------------------------------------------------------
      for ( $i = 0; $i < count($variants); $i++ ) { 
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
    $outJ["stdout"] = "Created test $test_name";
  }
  //------------------------------------------
  $http_code = 200;
  $outJ["status_code"] = $http_code;
  $outJ["msg_stdout"] = "Test [$test_name] with ID [$test_id] $action";
  $outJ["TestID"] = $test_id;
  $Y['msg_stdout']  = $outJ["msg_stdout"];
  $Y['status_code'] = $outJ["status_code"];
  db_set_row("log_ui_to_webapp", $request_webapp_id, $Y);
  // Note it is possible for both msg_stdout and msg_stderr to be set
  if ( $state == "started" ) {
    $status = inform_rts($test_id, $rts_err_msg);
    if ( !$status ) { 
      $http_code = 400; 
      $Y['msg_stderr'] = $rts_err_msg;
    }
  }

  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}

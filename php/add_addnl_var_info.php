<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
require_once 'db_get_test.php';
require_once 'mod_cell.php';
require_once 'lkp.php';

function add_addnl_var_info(
  $str_inJ
)
{
  //-- START: For logging
  $ret_val = start_log($str_inJ);
  $created_at = $ret_val['created_at'];
  $updated_at = $ret_val['updated_at'];
  $t_create   = $ret_val['t_create'];
  $t_update   = $ret_val['t_update'];
  $api_id     = $ret_val['api_id'];
  $request_webapp_id = $ret_val['request_webapp_id'];
  //-- STOP: For logging
  // START Check inputs
  assert(isset($str_inJ));
  assert(is_string($str_inJ));
  $inJ = json_decode($str_inJ); assert(!is_null($inJ));

  $test_id = get_json_element($inJ, 'id'); 
  $updater    = get_json_element($inJ, 'Updater');
  $updater_id = lkp("admin", $updater);
  $vid = get_json_element($inJ, 'VariantID'); 

  $variants  = get_json_element($inJ, 'Variants');
  $found = false;
  $custom_data = "";
  $description = "";
  foreach ( $variants as $v ) { 
    if ( $v->{'id'} != $vid )  { 
      continue; 
    }
    rs_assert($found == false); 
    $description = trim($v->{'description'});
    $custom_data = trim($v->{'custom_data'});
    $vname = trim($v->{'name'});
    $found = true;
    break;
  }
  rs_assert($found == true);
  //-- check input data 
  rs_assert(strlen($description) <= lkp("configs", "max_len_variant_dscr"));
  rs_assert(strlen($custom_data) <= lkp("configs", "max_len_custom_data"));

  // TODO P3: does description/custom data get set to null or empty string?

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");
  $test_name = $T['name'];
  $test_type = lkp("test_type", $T['test_type_id'], "reverse");
  $state     = lkp("state",     $T['state_id'], "reverse");
  if ( $custom_data != "" ) {  // must be valid JSON
    rs_assert(json_decode($custom_data));
  }
  // START: Database write
  $dbh = dbconn(); assert(!empty($dbh)); 
  try {
    $dbh->beginTransaction();
    mod_cell("variant", "description", $description, "id = $vid");
    mod_cell("variant", "custom_data", $custom_data, "id = $vid");
    $dbh->commit();
  } catch ( PDOException $ex ) {
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  // STOP: Database write
  //------------------------------------------
  $http_code = 200;
  $Y['status_code'] = $outJ["status_code"] = $http_code;
  $Y['msg_stdout'] = $outJ["msg_stdout"] = 
    "Variant [$vname, $vid] of Test [$test_name, $test_id] updated";
  if ( $state == "started" ) {
    $status = inform_rts($test_id, $rts_err_msg);
    if ( !$status ) { 
      $http_code = 400; 
      $Y['msg_stderr'] = $rts_err_msg;
    }
  }
  db_set_row("request_webapp", $request_webapp_id, $Y);
  header("Error-Code: $http_code");
  http_response_code($http_code);
  return $outJ;
}
?>

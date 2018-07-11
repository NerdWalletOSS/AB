<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");
require_once 'db_get_test.php';
require_once 'mod_cell.php';
require_once 'lkp.php';
require_once 'is_valid_json.php';
require_once 'get_json_element.php';
require_once 'aux_chk_name.php';

require_once 'start_log.php'; // UTPAL: Added the required file
require_once 'mod_row.php';  // UTPAL: Added the required file
require_once 'inform_rts.php';  // UTPAL: Added the required file

function add_addnl_var_info(
  $str_inJ
)
{
  //-- START: For logging
  $ret_val = start_log($str_inJ, 'additional_variant_info');
  $created_at = $ret_val['created_at'];
  $updated_at = $ret_val['updated_at'];
  $t_create   = $ret_val['t_create'];
  $t_update   = $ret_val['t_update'];
  $api_id     = $ret_val['api_id'];
  $request_webapp_id = $ret_val['request_webapp_id'];
  //-- STOP: For logging
  // START Check inputs
  rs_assert(isset($str_inJ));
  rs_assert(is_string($str_inJ));
  $inJ = json_decode($str_inJ); rs_assert(!is_null($inJ));

  $test_id = get_json_element($inJ, 'id'); 
  $updater    = get_json_element($inJ, 'Updater');
  // TODO P4 $updater_id = lkp("admin", $updater);
  $vid = get_json_element($inJ, 'VariantID'); 

  $T = db_get_row("test", "id", $test_id);
  rs_assert($T, "test [$test_id] not found");
  $test_name = $T['name'];
  $test_type = lkp("test_type", $T['test_type_id'], "reverse");
  $state     = lkp("state",     $T['state_id'], "reverse");
  rs_assert(( $state != "terminated" ) &&( $state != "archived" ));

  $variants  = get_json_element($inJ, 'Variants');
  $found = false;
  $custom_data = "";
  $description = "";
  $url         = "";
  foreach ( $variants as $v ) { 
    if ( $v->{'id'} != $vid )  { 
      continue; 
    }
    rs_assert($found == false); 
    if ( isset($v->{'custom_data'} ) ) {
      $custom_data = trim($v->{'custom_data'});
    }
    if ( isset($v->{'description'} ) ) {
      $description = trim($v->{'description'});
    }
    if ( isset($v->{'url'} ) ) {
      $url         = trim($v->{'url'});
    }
    $vname = trim($v->{'name'});
    $found = true;
    break;
  }
  rs_assert($found == true);
  //-- check input data 
  rs_assert(strlen($description) <= lkp("configs", "max_len_variant_dscr"));
  // check custom data 
  rs_assert(strlen($custom_data) <= lkp("configs", "max_len_custom_data"));
  if ( $custom_data != "" ) {  // must be valid JSON
    rs_assert(is_valid_json($custom_data));
  }
  // check url 
  if ( $test_type == "XYTest" ) {
    rs_assert(strlen($url)         > 1, "URL cannot be null");
    // UTPAL START: Shifted the closed curly bracket of if condition to line 85 to include all checks corresponding to URLs
    rs_assert(strlen($url)         <= lkp("configs", "max_len_url"));
    rs_assert(chk_url_text($url), "URL [$url] contains bad characters\n");
    $is_chk = lkp('configs', "check_url_reachable");
    if ( $is_chk ) { 
      rs_assert(chk_url($url), "URL [$url] not reachable\n");
    }
  // Included the following check inside the if condition of XYTest
  $V = db_get_rows("variant", " test_id= $test_id ");
    foreach ( $V as $v ) { 
    if ( $v['id'] == $vid ) { continue; };
    rs_assert($v['url'] != $url, "URL should be unique");
    }
  }
  // UTPAL END
  //

  if ( $description == "" ) { $description = "__NULL__"; } // TODO FIX
  if ( $custom_data == "" ) { $custom_data = "__NULL__"; } // TODO FIX
  if ( $url         == "" ) { $url         = "__NULL__"; } // TODO FIX
  $X1['description'] = $description;
  $X1['custom_data'] = $custom_data;
  $X1['url']         = $url;
  // TODO P4 $X1['updater_id']  = $updater_id;
  $X1['api_id']      = $api_id;
  $X1['request_webapp_id']      = $request_webapp_id;
  // START: Database write
  $dbh = dbconn(); rs_assert(!empty($dbh)); 
  try {
    $dbh->beginTransaction();
    mod_row("variant", $X1, " where id = $vid");
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
  if ( $state == "started" ) {
    $status = inform_rts($test_id, $rts_err_msg);
    if ( !$status ) { 
      $http_code = 400; 
      $Y['msg_stderr'] = $rts_err_msg;
    }
  }
  $Y['status_code'] = $outJ["status_code"] = $http_code;
  $Y['msg_stdout'] = $outJ["msg_stdout"] = 
    "Variant [$vname, $vid] of Test [$test_name, $test_id] updated";
  $outJ["TestID"] = $test_id; // UTPAL: Added this line as after the completion, I need the test ID back to display the page.
  $Y['msg_stderr']  = $outJ["msg_stderr"] = $err;
  db_set_row("request_webapp", $request_webapp_id, $Y);
  header("Error-Code: $http_code");
  header("Error-Message: ".$err);
  header("Description: <<$description>>");
  http_response_code($http_code);
  return $outJ;
}
?>

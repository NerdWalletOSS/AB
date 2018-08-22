<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");
set_include_path(get_include_path() . PATH_SEPARATOR . "./rts/");
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
require_once 'start_log.php';
require_once 'dup_row.php';
error_reporting(E_ALL);

function test_basic(
  $str_inJ
)
{
  //-- START: For logging
  $ret_val = start_log($str_inJ, "insert_test_edit_test_basic");
  $created_at = $ret_val['created_at'];
  $updated_at = $ret_val['updated_at'];
  $t_create   = $ret_val['t_create'];
  $t_update   = $ret_val['t_update'];
  $api_id     = $ret_val['api_id'];
  $request_webapp_id = $ret_val['request_webapp_id'];
  //-- STOP: For logging

  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); 
  rs_assert(gettype($inJ) != "string");
  rs_assert($inJ, "invalid JSON");
  $test_name = get_json_element($inJ, 'name'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  //-----------------------------------------------
  $is_overwrite = false;
  $x         = get_json_element($inJ, 'OverWriteURL', false);
  if ( $x === "true" ) {
    $is_overwrite = true;
  }
  $state     = get_json_element($inJ, 'State', false);
  $test_name = get_json_element($inJ, 'name'); 
  $test_type = get_json_element($inJ, 'TestType'); 
  $test_dscr = get_json_element($inJ, 'description', false);
  if ( empty($test_dscr) || ( $test_dscr == "NULL" ) ) {
    $test_dscr = "__NULL__"; // TODO FIX
  }
  else {
    rs_assert(strlen($test_dscr) <= lkp('configs', "max_len_test_dscr"));
  }

  $variants  = get_json_element($inJ, 'Variants');
  // TODO P4 later $bin_type  = get_json_element($inJ, 'BinType', false);
  $channel   = get_json_element($inJ, 'Channel', false);
  $channel_id = null;
  $pred_id    = null;
  if ( !empty($channel) ) {
    $channel_id = lkp("channel", $channel);
  }
  //-------------------------------------------------
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
    $T = db_get_row("test", "id", $test_id);
    rs_assert($T, "No test with id = $test_id \n");
    $updater    = get_json_element($inJ, 'Updater');
    $updater_id = lkp("admin", $updater);
    $is_dev_specific = $T['is_dev_specific'];
    // On this page, pred_id cannot be set but it can be broken
    if ( $channel_id != $T['channel_id'] ) { 
      $pred_id = "__NULL__";
    }
  }
  if ( empty($channel) ) {
    $channel_id = "__NULL__";
  }
  //-------------------------------------------------
  if ( $is_new ) {
    switch ( $test_type ) {
    case "ABTest" :
      $bin_type =  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok";
      break;
    case "XYTest" :
      $bin_type = "anonymous";
      break;
    default : 
      rs_assert(null, "Invalid test type $test_type");
      break;
    }
    $inJ->{'State'}   = $state = "draft";
    $inJ->{'BinType'} = $bin_type;
  }
  else {
    $bin_type = get_json_element($inJ, 'BinType');
  }
  rs_assert($state, "State should be known by now");
  rs_assert(( ( $state != "terminated" ) && ( $state != "archived" ) ));
  rs_assert($bin_type, "Bin type not set ");
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
  //---------------------------------------------_-------
  $X1['request_webapp_id']  = $request_webapp_id;
  $X1['api_id']       = $api_id;
  $X2['request_webapp_id']  = $request_webapp_id;
  $X2['api_id']       = $api_id;
  $X3['request_webapp_id']  = $request_webapp_id;
  $X3['api_id']       = $api_id;
  $X4['request_webapp_id']  = $request_webapp_id;
  $X4['api_id']       = $api_id;
  $X5['request_webapp_id']  = $request_webapp_id;
  $X5['api_id']       = $api_id;
  if ( $test_id > 0 ) {  // update
    if ( $pred_id ) { 
      $X1['pred_id'] == $pred_id;
    }
    $action = "updated";
    $state = get_json_element($inJ, 'State');
    rs_assert($state != "archived");  // no changes to archived state
    $X1['description']  = $test_dscr;
    $X1['channel_id']   = $channel_id; // UTPAL: To allow channel editing 
    $X1['updated_at']   = $updated_at;
    $X1['t_update']     = $t_update;
    $X1['updater_id']   = $updater_id;
    if ( $state == "draft" ) { 
      $X1['name']  = $test_name;
    }
    //-----------------------------------------------
    $dbh = dbconn(); rs_assert(isset($dbh)); 
    try {
      $dbh->beginTransaction();
      //--- Update test table 
      mod_row("test", $X1, "where id = $test_id ");
      //--- Update variant table 
      $X2['t_update'] = $t_update;
      $X2['updated_at'] = $updated_at;
      for ( $i = 0; $i < count($variants); $i++ ) {
        // can change name only in dormant mode 
        if ( $state == "draft" ) {
          $X2['name']        = $variant_names[$i];
        }
        if ( ( $state == "draft" ) || ( $state == "dormant" ) ||
          ( $state == "started" ) ) {
            if ( !$is_dev_specific ) { 
              // can change percentage only if not device specific
              $X2['percentage']  = $variant_percs[$i];
            }
            if ( $test_type == "XYTest" ) {
              $X2['url']        = $variant_urls[$i];
            }
            if ( ( $test_type == "XYTest" ) && ( $state == "started" ) ) { 
              $R = db_get_row("variant", "id", $variant_ids[$i]);
              $old_url = $R['url'];
              $new_url = $variant_urls[$i];
              if ( $old_url != $new_url ) { 
                $new_variant_id  = dup_row("variant", $variant_ids[$i],
                  array( "is_del" => true), 
                  array( "url" => $new_url));
                // Update device_x_variant table???
                $X6['variant_id'] = $new_variant_id;
                mod_row("device_x_variant", $X6, 
                 "  where variant_id = " . $variant_ids[$i]);
                $variant_ids[$i] = $new_variant_id;
              }
            }
          }
        mod_row("variant", $X2, "where id = " . $variant_ids[$i]);
      }
      //--- Update device_x_variant table --------
      if ( !$is_dev_specific ) { 
        // can change percentage only if not device specific
        $D = db_get_rows("device");
        foreach ( $D as $d ) { 
          $device_id = $d['id'];
          for ( $i = 0; $i < count($variants); $i++ ) {
            $variant_id       = $variant_ids[$i];
            $X3['percentage'] = $variant_percs[$i];
            mod_row("device_x_variant", $X3, 
              " where variant_id = $variant_id and device_id = $device_id ");
          }
        }
      }
      //------------------------------------------
      $dbh->commit();
    } catch ( PDOException $ex ) {
      $dbh->rollBack();
      rs_assert(false, "ERROR: Transaction aborted");
    }
  } 
  else { // insert
    $action = "inserted";
    $state = "draft";
    $X1['name']         = $test_name;
    $X1['description']  = $test_dscr;
    $X1['test_type_id'] = $test_type_id;
    $X1['channel_id']   = $channel_id;
    $X1['seed']         = make_seed();
    $X1['external_id']  = $t_create;
    $X1['created_at']   = $created_at;
    $X1['t_create']     = $t_create;
    $X1['updated_at']   = $updated_at;
    $X1['t_update']     = $t_update;
    $X1['creator_id']   = $creator_id;
    $X1['updater_id']   = $creator_id;
    $X1['state_id']     = $state_id;
    $X1['bin_type_id'] = $bin_type_id;
    //-----------------------------------------------

    $dbh = dbconn(); rs_assert(!empty($dbh)); 
    try {
      $dbh->beginTransaction();
      //---- Insert into test table 
      unset($test_id);
      $test_id = insert_row("test", $X1);
      //---- Insert into variant table 
      unset($variant_ids); $vidx = 0;
      $X2['test_id']  = $test_id;
      $X2['t_update'] = $t_update;
      $X2['updated_at'] = $updated_at;
      for ( $i = 0; $i < count($variants); $i++ ) { 
        $X2['percentage']  = $variant_percs[$i];
        $X2['name']        = $variant_names[$i];
        $X2['url']         = $variant_urls[$i];
        $variant_ids[$vidx] = insert_row("variant", $X2);
        $vidx++;
      }
      //--- Insert into device_x_variant table --------
      $D = db_get_rows("device");
      $X3['test_id'] = $test_id;
      foreach ( $D as $d ) { 
        $X3['device_id'] = $d['id'];
        for ( $i = 0; $i < count($variants); $i++ ) {
          $X3['variant_id'] = $variant_ids[$i];
          $X3['percentage'] = $variant_percs[$i];
          $variant_id = insert_row("device_x_variant", $X3);
        }
      }
      //------------------------------------------
      //--- Insert into cat_attr_val_test table
      $attr_type_id = lkp("attr_type", "categorical");
      rs_assert($attr_type_id);
      $A = db_get_rows("attr", "((attr_type_id = $attr_type_id) and (is_del = false))");
      if ( !empty($A) ) { 
        foreach ( $A as $a ) { 
          //var_dump($a);
          $attr_id = $a['id'];
          $CV = db_get_rows("cat_attr_val", 
            "((attr_id = $attr_id) and (is_del = false))");
          foreach ( $CV as $cv ) { 
            $X5['cat_attr_val_id'] = $cv['id'];
            $X5['test_id'] = $test_id;
            $X5['attr_id'] = $attr_id;
            insert_row("cat_attr_val_test", $X5);
          }
        }
      }
      //------------------------------------------
      $dbh->commit();
    } catch ( PDOException $ex ) {
      $dbh->rollBack();
      rs_assert(false, "ERROR: Transaction aborted");
    }
  }
  //------------------------------------------
  $http_code = 0; 
  if ( $state == "started" ) {
    $http_code = 200; 
    $rts_error_msg = "";
    $status = inform_rts($test_id, $rts_err_msg);
    if ( !$status ) {$http_code = 400; $outJ['msg_stderr'] = $rts_err_msg;}
  }
  $outJ["rts_code"] = $http_code;
  $outJ["msg_stdout"] = "Test [$test_name] with ID [$test_id] $action";

  $outJ["TestID"] = $test_id;
  $outJ["OverWrite"] = $is_overwrite;
  header("TestID: $test_id"); // just for Lua test cases

  // Note that state cannot be terminated or archived for this endpoint
  return $outJ;
  /* always PHP code called from front-end returns outJ which has
   * 1) msg_stdout
   * 2) rts_code, 200 is good, anything else is bad
   * Note one exception: 0 means that the RTS was not called at all
      header("TestID: $test_id"); // just for Lua test cases
      NO other place in code has header(..)
   * Anything custom to the page e.g.
   * 1) TestID
   * 2) IsOverWrite
   * If there is a PHP error, rs_assert() will happen
   * If there was no rs_assert => all is well
   * All PHP endpoints will return a table called outJ. 
   * Should never have any return statement in code except at end
   * PHP code interacts with RTS through inform_rts() nothing else
   *
   *
   * For functions that do not interact with RTS (add_admin), we follow 
   * same protocol as above except that rts_code is always 0
   *
   * For chk_test() and chk_tests(), 
   * we either return true or we rs_assert out
   * For chk_url() 
   * we either return true or we return false or we rs_assert out
   * For db_get_test() 
   * we either return a table or we rs_assert out 
   * For get_url(), let us discuss further.
   * */
}

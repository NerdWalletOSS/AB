<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "load_configs.php";
require_once "name_to_id.php";
require_once "is_valid_json.php";
function new_set_description(
  $test_name,
  $test_type,
  $variant, /* set to empty string if you want to modify test description */
  $modifier,
  $what_to_modify, // test_dscr, var_dscr, var_cust_data
  $custom_data = "",
  $description = "",
  $regex = ""
)
{
  $test_name = trim($test_name);
  $test_type = trim($test_type);
  $variant   = trim($variant);
  $modifier  = trim($modifier);
  $custom_data = trim($custom_data);
  $description = trim($description);
  $regex       = trim($regex);
  //----------------------------------------
  $GLOBALS["err"] = "";
  $dbh = dbconn();
  if ( !$dbh ) {
    $GLOBALS["err"] .= "Cannot connect to database\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //---------------------------------------
  if ( !load_configs() ) {
    $GLOBALS["err"] .= "Unable to load configs \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  $configs = $GLOBALS['CONFIGS'];
  $max_len_test_dscr     = $configs['max_len_test_dscr'];
  $max_len_variant_dscr  = $configs['max_len_variant_dscr'];
  $max_len_custom_data   = $configs['max_len_custom_data'];
  $max_len_regex         = $configs['max_len_regex'];
  //-------------------------------------------------------
  $test_type_id = name_to_id("test_type", "name", $test_type);
  if ( !$test_type_id ) {
    $GLOBALS["err"] .= "Bad test_type = [$test_type]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------------
  $modifier_id   = name_to_id("admin", "name", $modifier);
  if ( !$modifier_id ) {
    $GLOBALS["err"] .= "Bad modifier = [$modifier]\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------------
  $archived_id   = name_to_id("state", "name", "archived");
  $T = db_get_row("abtest", "name", $test_name,
    " and test_type_id = $test_type_id and state_id != $archived_id ");
  if ( !$T ) {
    $GLOBALS["err"] .= "No test [$test_name] of type $test_type\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $creator_id = $T['creator_id'];
  $state_id   = $T['state_id'];
  $state = id_to_name($state_id, "state");
  $is_private = $T['is_private'];
  $abtest_id  = $T['pk'];
  //-----------------------------------
  if ( $variant != "" ) {
    $V = db_get_row("variant", "abtest_id",  $abtest_id,
      " and name = '$variant' ");
    if ( !$V ) {
      $GLOBALS["err"] .= "Unknown Variant [$variant] \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $variant_id = $V['pk'];
  }
  //-------------------------------------------------------
  if ( ( $is_private ) && ( $creator_id != $modifier_id ) )  {
    $GLOBALS["err"] .= "Cannot modify private test \n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //-------------------------------------------------------
  $sql = ""; // should be set before prepared statement executes
  switch ( $what_to_modify ) {
  case "TestDescription" :
    // We are modifying test description
    if ( strlen($description) > $max_len_test_dscr ) {
      $GLOBALS["err"] .= "Test Description too long \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $sql = "update abtest set description = :description where pk = :pk";
    $X['description'] = $description;
    $X['pk'] = $abtest_id;
    break;
  case "RegEx" :
    if ( strlen($regex) > $max_len_regex ) {
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    if ( ( $test_type == "ABTest" ) && ( $state == "dormant" ) ) {
    // Can be done only for dormant AB Tests
    }
    else {
      $GLOBALS["err"] .= "RegEx settable only for dormant AB Tests\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $sql = "update abtest set regex = :regex where pk = :pk";
    $X['regex'] = $regex;
    $X['pk']    = $abtest_id;
    break;
  case "VariantDescription" :
    // We are modifying variant description
    if ( strlen($description) > $max_len_variant_dscr ) {
      $GLOBALS["err"] .= "Variant Description too long \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $sql = "update variant set description = :description where pk = :pk";
    $X['description'] = $description;
    $X['pk']          = $variant_id;
    // For ABTest, Cannot modify description of Control variant
    if ( ( $test_type == "ABTest" ) && ( $V['name'] == "Control" ) ) {
      $GLOBALS["err"] .= "Cannot change description of Control \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    // For XYTest, Cannot modify description once test starts
    if ( ( $test_type == "XYTest" ) && ( $state != "dormant" ) ) {
      $GLOBALS["err"] .= "Cannot change description once test starts\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    break;
  case "VariantCustomData" :
    if ( $custom_data != "" ) {
      $Y = json_decode($custom_data);
      if ( !$Y ) {
        $GLOBALS["err"] .= "Custom data not valid JSON<<$custom_data>>\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    }
    //-------------------------------------------------------
    if ( $test_type == "XYTest" ) {
      $GLOBALS["err"] .= "Cannot set custom data for XYTest\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    //-------------------------------------------------------
    $rslt = is_valid_json($custom_data);
    if ( !$rslt ) {
      $GLOBALS["err"] .= "Invalid json for variant $variant \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    //-------------------------------------------------------
    if ( strlen($custom_data) >  $max_len_custom_data ) {
      $GLOBALS["err"] .= "Custom Data too long \n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    $sql = "update variant set custom_data = :custom_data where pk = :pk";
    $X['custom_data'] = $custom_data;
    $X['pk']          = $variant_id;
    break;
  default :
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
    break;
  }
  if ( $sql == "" ) { return false; } // nothing to do
  $stmt = $dbh->prepare($sql);
  try {
    $dbh->beginTransaction();
    $rslt = $stmt->execute($X);
    if ( !$rslt ) { throw new Exception("statement $stmt failed"); }
    $dbh->commit();
  } catch ( PDOException $ex ) {
    var_dump($ex);
    $dbh->rollBack();
    $GLOBALS["err"] .= "ERROR: Transaction aborted\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  return true;
}
?>

<?php
require_once "db_get_variants.php";
require_once "name_to_id.php";
require_once "db_get_row.php";

function mk_add_test_url(
  $test_name,
  $test_type
)
{
  $test_type_id = name_to_id("test_type", "name", $test_type);
  if ( !$test_type_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $archived_id = name_to_id("state", "name", "archived");
  if ( !$archived_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $abtest_id = name_to_id("abtest", "name", $test_name);

  $T = db_get_row("abtest", "name", $test_name, 
    " and test_type_id = $test_type_id and state_id != $archived_id ");
  if ( !$T ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $abtest_id     = $T['pk'];
  $external_id   = $T['external_id'];
  $uses_eee      = $T['uses_eee'];
  $is_sequential = $T['is_sequential'];
  $is_dev_specific = $T['is_dev_specific'];
  $seed          = $T['seed'];

  $variants = db_get_variants($abtest_id);
  $num_variants = count($variants);

  $url  = "AddTest?";
  $url .= "TestName=$test_name";
  $url .= "&TestType=$test_type";
  $url .= "&Seed=$seed";
  $url .= "&UsesEEE=$uses_eee";
  $url .= "&IsSequential=$is_sequential";
  $url .= "&IsDevSpecific=$is_dev_specific";
  $url .= "&TestID=$abtest_id";
  if ( $test_type == "XYTest" ) { 
    $url .= "&ExternalID=$external_id";
  }
  $url .= "&NumVariants=$num_variants";
  $idx = 0;
  foreach ( $variants as $v ) { 
    $name = $v['name'];
    $perc = $v['percentage'];
    $id   = $v['pk'];
    $url .= "&Variant$idx=$name:$perc";
    $url .= "&VariantID$idx=$id";
    if ( $name == "Control" ) { 
     $url .= "&ControlID=$id";
    }
    $idx++;
  }
  return $url;
}
?>

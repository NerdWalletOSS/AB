<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "db_set_row.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "aux_check_name.php";
require_once "db_get_variants.php";
require_once "xurl.php";
require_once "chk_url.php";
require_once "chk_is_sequential.php";
require_once "load_configs.php";

##C \begin{itemize}
##C \item Add one row to Table abtest
##C \item Add \(n\) row to Table variant, where \(n-1\) is number of user 
##C specified variants, 1 being for Control
##C \item Add \(n\) rows Table ramp\_history, one for each variant
##C \item Add 1 row to Table state\_history
##X \end{itemize} 
//<hdr>
function chk_add_test(
  &$test_name, // name of test, mandatory
  &$test_type, // ABTest, XYTest
  &$test_text, // description, optional
  &$creator,   // name of user who is creating test
  $variant_names, // array: names of variants
  $variant_percs, // array: percentages of variants
  $variant_texts, // array: description of variants
  $channel,
  $is_sequential
)
//</hdr>
{
  $GLOBALS["err"] = "";
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true; 
  }
  $configs = $GLOBALS['CONFIGS'];
  $min_num_variants      = $configs['min_num_variants'];
  $max_num_variants      = $configs['max_num_variants'];
  $max_len_test_name     = $configs['max_len_test_name'];
  $max_len_variant_name  = $configs['max_len_variant_name'];
  $max_len_test_dscr     = $configs['max_len_test_dscr'];
  $max_len_variant_dscr  = $configs['max_len_variant_dscr'];
  $max_num_tests         = $configs['max_num_tests'];
  $server                = $configs['server'];
  $port                  = $configs['port'];
  //---------------------------------------
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "Cannot connect to database\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C Before we add a test, the input must satisfy the following
  ##C \begin{enumerate}
  ##C \item test type is ABTest or XYTest 
  $test_type = trim($test_type); 
  if ( ( $test_type != "ABTest" ) && ( $test_type != "XYTest" ) ) {
    $GLOBALS["err"] .= "Test Type [$test_type] is invalid\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item If test_type is XTTest, is_sequential is false
  if ( ( $test_type == "XYTest" ) && ( $is_sequential ) ) {
    $GLOBALS["err"] .= "Cannot specify is_sequential for XYTest\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item If test_type is ABTest, channel must be empty string
  if ( ( $test_type == "ABTest" ) && ( $channel != "" ) ) { 
    $GLOBALS["err"] .= "Cannot specify channel for ABTest\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item If test_type is XYTest and channel is not empty string
  ##C       then it must point to valid channel
  if ( ( $test_type == "XYTest" ) && ( $channel != "" ) ) { 
    $channel_id = name_to_id("channel", "name", $channel);
    if ( !$channel_id )  {
      $GLOBALS["err"] .= "Cannot specify channel for ABTest\n"; //TODO: Message should be "No such channel exist".
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
  }
  ##C \item Test name cannot be null
  $test_name = trim($test_name); 
  if ( $test_name == "" ) {
    $GLOBALS["err"] .= "Test Name is null\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item Test name restricted to alphanumeric and underscore
  if ( !aux_check_name($test_name) ) {
    $GLOBALS["err"] .= "Test Name restricted to alphanumeric, underscore\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  ##C \item Test name length restricted 
  if ( strlen($test_name) > $max_len_test_name ) { 
    $GLOBALS["err"]  = "Test name too long [$test_name]\n.";
    $GLOBALS["err"] .= "Max length is $max_len_test_name\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item Creator of test must be listed in admin table 
  $rslt = db_get_row("admin", "name", $creator); 
  if ( is_null($rslt) ) { 
    $GLOBALS["err"] .= "No User [$creator]."; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  $creator_id = $rslt['pk'];
  ##C \item Test name not reserved word 
  $rslt = db_get_row("reserved_words", "name", strtolower($test_name));
  if ( !is_null($rslt) ) { 
    $GLOBALS["err"] .= "Test name is reserved word [$test_name].\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item Test Text must be within bounds. Can be null
  if ( strlen($test_text) > $max_len_test_dscr ) {
    $GLOBALS["err"] .= "Test description too long = [$test_text].\n";
    $GLOBALS["err"] .= "Should be less than $max_len_test_dscr characters";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item variant names is an array 
  if ( !is_array($variant_names) ) {
    $GLOBALS["err"] .= "A: Invalid variant_names\n"; 
    return false; 
  }
  ##C \item variant percentages is an array 
  if ( !is_array($variant_percs) ) { 
    $GLOBALS["err"] .= "A: Invalid variant_percs\n"; 
    return false; 
  }
  ### \item variant descriptions is an array 
  if ( !is_array($variant_texts) ) { 
    $GLOBALS["err"] .= "A: Invalid variant_texts\n"; 
    return false; 
  }
  ##C \item size of perecntages array same as names array 
  $nV = count($variant_names);
  if ( $nV != count($variant_percs ) ) { 
    $GLOBALS["err"] .= "B: Invalid variant_percs\n"; 
    return false; 
  }
  ##C \item size of perecntages array same as descriptions array 
  if ( $nV != count($variant_texts ) ) { 
    $GLOBALS["err"] .= "C: Invalid variant_texts\n"; 
    return false; 
  }
  ##C \item number of variants in bounds 
  if (($nV < $min_num_variants ) || ($nV > $max_num_variants ) ) { 
    $GLOBALS["err"] .= "Num variants = $nV out of bounds\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item length of variant description should be in bounds 
  foreach ( $variant_texts as $vt ) {
    if ( strlen($vt) > $max_len_variant_dscr ) { 
      $GLOBALS["err"]  = "Variant description too long = [$vt].\n";
      $GLOBALS["err"] .= "Should be less than $max_len_variant_dscr characters";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
  }
  // Convert some strings to primary keys 
  $test_type_id = name_to_id("test_type", "name", $test_type);
  if ( !$test_type_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }

  $dormant_id = name_to_id("state", "name", "dormant");
  if ( !$dormant_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $dormant_id = $rslt['pk'];

  $archived_id = name_to_id("state", "name", "archived");
  if ( !$archived_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }

  $started_id = db_get_row("state", "name", "started");
  if ( !$started_id ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }

  ##C \item Adding this test will not exceed maximum 
  $actual_num_tests = db_num_rows("abtest", "state_id != $archived_id  ");
  if ( $actual_num_tests >= $max_num_tests ) { 
    var_dump($max_num_tests);
    var_dump($actual_num_tests);
    $GLOBALS["err"] .= "No more tests. Limit is $max_num_tests\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //--------------------------------------
  // TODO: Document: for unique test names we do case insensitve comparison
  ##C \item There is not an existing un-archived test with same name 
  $lwr_name = strtolower($test_name);
  $rslt = db_get_row("abtest", "name", $lwr_name, 
    " and test_type_id = $test_type_id and state_id != $archived_id"); 
  if ( $rslt ) {  // TODO P1 Check this condition
    $GLOBALS["err"] .= "Test $test_name already exists.\n"; 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false; 
  }
  ##C \item Percentages of control and variants meet following 
  ##C \begin{enumerate}
  $sum = 0;
  for ( $i = 0; $i < $nV; $i++ ) {
    ##C \item Percentage must be an integer 
    if ( ( !is_numeric($variant_percs[$i]) ) || 
      ( (int)($variant_percs[$i] ) != $variant_percs[$i] ) ) {
        $GLOBALS["err"] .= "Bad var perc {$variant_percs[$i]}\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    ##C \item Percentage must be 0 and 100 
    if ( ( $variant_percs[$i] < 0 ) || 
      ( $variant_percs[$i] > 100 ) ) {
        $GLOBALS["err"] .= "Bad var perc [{$variant_percs[$i]}]\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    $sum += $variant_percs[$i];
    ##C \item Variant name contains only alphanumeric and underscore
    if ( !aux_check_name($variant_names[$i]) ) { 
      $GLOBALS["err"]  = "Bad var name [{$variant_names[$i]}]\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    ##C \item Variant name not null
    if ( strlen($variant_names[$i]) == 0 ) { 
      $GLOBALS["err"] .= "Var name too short [{$variant_names[$i]}]\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    ##C \item Variant name does not exceed max length 
    if ( strlen($variant_names[$i]) > $max_len_variant_name )  {
      $GLOBALS["err"] .= "Var name too long {$variant_names[$i]}\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false;
    }
    ##C \item Variant name not reserved word 
    $lwr_name_i = strtolower($variant_names[$i]);
    if ( ( $test_type == "ABTest" ) && ( $i == 0 ) ) {
      if ( $variant_names[$i] != "Control" ) { 
        $GLOBALS["err"] .= "For ABTest, Variant 0 must be Control\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    }
    else {
      $rslt = db_get_row("reserved_words", "name", $lwr_name_i);
      if ( $rslt ) { 
        $GLOBALS["err"] .= "Variant name cannot be reserved word = $lwr_name_i\n";
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
    }
    ##C \item No duplicates in variant names 
    for ( $j = $i+1; $j < $nV; $j++ ) { 
      if ( $lwr_name_i == strtolower($variant_names[$j]) ) { 
        $GLOBALS["err"] .= "Duplicate variant names $lwr_name_i ";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false;
      }
    }
  }
  ##C \item Sum of variants must be 100 
  if ( $sum != 100 ) { 
    $GLOBALS["err"] .= "Variant percentages should add to 100 not  " . $sum . "\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  ##C \end{enumerate}
  ##C \begin{enumerate}
  ##C Check that exactly one variant has 100 
  if ( ( $is_sequential ) && ( !chk_is_sequential($variant_percs, $nV) ) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  ##C \end{enumerate}
  ##C \begin{enumerate}
  ##C \item ABTests must pass some additional checks 
  ##C \begin{enumerate} 
  if ( $test_type == "ABTest" ) { 
    if ( $variant_names[0] != "Control" ) { 
      ##C \item Variant 0 name must be Control";
      $GLOBALS["err"] .= "For ABTest, Variant 0 name must be Control\n";
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
    if ( $variant_texts[0] != "Control" ) { 
      ##C \item Variant 0 text must be Control";
      $GLOBALS["err"] .= "For ABTest, Variant 0 text must be Control\n";
      $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
      return false; 
    }
    if ( $is_sequential == 1 ) { 
      // this additional test does not app;y for sequential tests
    }
    else {
      for ( $i = 1; $i < $nV; $i++ ) { // all but Control
        ##C \item percentage of no conotrl <= \(\frac{100}{nV-1}\)
        if ( $variant_percs[$i] > ( 100 / ($nV-1) ) ) {
          $GLOBALS["err"] .= "Variant percentages screwed up\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false; 
        }
      }
    }
  }
  ##C \end{enumerate} 
  ##C \item XYTests have to pass the following tests 
  ##C \begin{enumerate}
  if ( $test_type == "XYTest" ) {
    ##C \item URL must be non-null, and not contail bad characters
    for ( $i = 0; $i < $nV; $i++ ) {
      if ( !chk_url_text($variant_texts[$i]) ) {
        $GLOBALS["err"] .= "Bad URL [${variant_texts[$i]}]\n"; 
        $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
        return false; 
      }
    }
    ##C \item landing pages must be unique 
    for ( $i = 0; $i < $nV; $i++ ) {
      for ( $j = $i+1; $j < $nV; $j++ ) { 
        if ( $variant_texts[$i] == $variant_texts[$j] ) {
          $GLOBALS["err"] .= "ERROR. Landing URLs need to be unique\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false; 
        }
      }
    }
    $check_url_reachable = $configs["check_url_reachable"];
    if ( $check_url_reachable != "false") {
      ##C \item landing pages should be reachable
      for ( $i = 0; $i < $nV; $i++ ) {
        $url  = $variant_texts[$i];
        $is_valid_url = false;
        $rslt = chk_url($url);
        if ( !$rslt ) { 
          $GLOBALS["err"] .= "URL [$url] is not reachable.\n";
          $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
          return false; 
        }
      }
    }
    else {
      $GLOBALS["err"] .= "WARNING. Not checking reachable URLs for XYTest\n";
    }
  }
  ##C \end{enumerate}
  ##C \end{enumerate}
  return true;
}
?>

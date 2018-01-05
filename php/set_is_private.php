<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "name_to_id.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_num_rows.php";
require_once "db_get_variants.php";
require_once "xurl.php";
require_once "chk_url.php";
require_once "chk_add_test.php";
require_once "load_configs.php";

//<hdr>
//C \section{Set IsPrivate}
//C \label{set_is_private}
//C Argumets are 
//C \begin{enumerate}
function set_is_private(
  $test_name, //C \item TestName
  $test_type, //C \item TestType
  $modifier,  //C \item Modifier
  $is_private //C \item IsPrivate
)
//C \end{enumerate}
//</hdr>
{
  $GLOBALS["err"] = "";
  $dbh = dbconn(); 
  // Load configs 
  if ( !load_configs() ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true; 
  }

  //C \begin{enumerare}
  $test_type_id = name_to_id("test_type", "name", $test_type);
  $archived_id  = name_to_id("state", "name", "archived");
  //----------------------------------------------------------------
  //C \item specified test must exist in non-archived state 
  $T = db_get_row("abtest", "name", $test_name, 
    " and state_id != $archived_id  and test_type_id = $test_type_id ");
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid Test [$test_name] \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $pk             = $T['pk'];
  $creator_id     = $T['creator_id'];
  $old_is_private = $T['is_private'];
  //----------------------------------------------------------------
  //C \item modifier must be legit admin
  $A = db_get_row("admin", "name", $modifier);
  if (!$A ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid admin [$modifier] "; return false;
  }
  //----------------------------------------------------
  //C \item modifier must be creator 
  $modifier_id = $A['pk'];
  if ( $modifier_id != $creator_id ) {
    $GLOBALS["err"] .= "User [$modifier] not creator\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //----------------------------------------------------
  //C \item IsPrivate must be 0 or 1
  if ( ( $is_private != 0 ) && ( $is_private != 1 ) ) {
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid is_private [$is_private] "; return false;
  }
  //----------------------------------------------------
  //C \item New value for IsPrivate must different from old. Else,
  //C not an error but quick return
  if ( $is_private == $old_is_private ) { 
    $GLOBALS["err"] .= "WARN: No change to be made \n";
    $GLOBALS["err"] .= "WARN: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  //C \end{enumerate}
  //----------------------------------------------------
  $d_create =  $d_change = get_date(); 
  $t_create =  $t_change = get_time_usec(); 
  $sql  = "update abtest set ";
  $sql .= "is_private = :is_private, ";
  $sql .= "most_recent_update = :d_create, ";
  $sql .= "t_update = :t_create where pk = :pk";
  $stmt = $dbh->prepare($sql);
  $X['pk']         = $pk;
  $X['d_create']   = $d_create;
  $X['t_create']   = $t_create;
  $X['is_private'] = $is_private;
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
/* quick and dirty test 
$test_name = "MJtpcN4SBEzsT91nLGYxexS6jdPQip7";
$test_type = "XYTest";
$modifier = "joe";
set_is_private($test_name, $test_type, $modifier, 0);
var_dump($GLOBALS['err']);
 */
?>

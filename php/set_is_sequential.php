<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../test/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");

require_once "bye.php";
require_once "name_to_id.php";
require_once "get_time_usec.php";
require_once "dbconn.php";
require_once "db_get_row.php";
require_once "db_get_variants.php";
require_once "chk_is_sequential.php";
require_once "load_configs.php";

//<hdr>
//C \section{Set IsSequential}
//C \label{set_is_sequential}
//C Argumets are 
//C \begin{enumerate}
function set_is_sequential(
  $test_name, //C \item TestName
  $test_type, //C \item TestType
  $modifier,  //C \item Modifier
  $is_sequential //C \item IsPrivate
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
  $dormant_id   = name_to_id("state", "name", "dormant");
  //----------------------------------------------------------------
  //C \item specified test must exist in non-archived state 
  $T = db_get_row("abtest", "name", $test_name, 
    " and state_id != $archived_id  and test_type_id = $test_type_id ");
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid Test [$test_name] \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $pk         = $T['pk'];
  $creator_id = $T['creator_id'];
  $state_id   = $T['state_id'];
  $is_private = $T['is_private'];
  $old_is_sequential = $T['is_sequential'];
  //C \item state must be dormant
  if ( $state_id != $dormant_id ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "State not dormant\n"; return false;
  }
  //----------------------------------------------------
  //----------------------------------------------------------------
  //C \item modifier must be legit admin
  $A = db_get_row("admin", "name", $modifier);
  if (!$A ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid admin [$modifier] "; return false;
  }
  //----------------------------------------------------
  //C \item modifier must be creator if private
  $modifier_id = $A['pk'];
  if ( (  $is_private == 1 ) && ( $modifier_id != $creator_id ) ) {
    $GLOBALS["err"] .= "User [$modifier] not creator\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //----------------------------------------------------
  //C \item IsSequential must be 0 or 1
  if ( ( $is_sequential != 0 ) && ( $is_sequential != 1 ) ) {
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid is_sequential [$is_sequential] "; 
    return false;
  }
  //----------------------------------------------------
  //C \item New value for IsPrivate must different from old. Else,
  //C not an error but quick return
  if ( $is_sequential == $old_is_sequential ) { 
    $GLOBALS["err"] .= "WARN: No change to be made \n";
    $GLOBALS["err"] .= "WARN: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return true;
  }
  //C \end{enumerate}
  //C \begin{enumerate}
  //C Existing variants prooprtions must pass chk_is_sequential
  $V = db_get_variants($pk);
  $nV = count($V); $percentages = array($nV); $idx = 0;
  foreach ( $V as $v ) { 
    $percentages[$idx++] = $v['percentage'];
  }
  if ( !chk_is_sequential($percentages, $nV) ) {
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  ##C \end{enumerate}
  //C \end{enumerate}
  //----------------------------------------------------
  $d_create =  $d_change = get_date(); 
  $t_create =  $t_change = get_time_usec(); 
  $sql  = "update abtest set ";
  $sql .= "is_sequential = :is_sequential, ";
  $sql .= "most_recent_update = :d_create, ";
  $sql .= "t_update = :t_create where pk = :pk";
  $stmt = $dbh->prepare($sql);
  $X['pk']         = $pk;
  $X['d_create']   = $d_create;
  $X['t_create']   = $t_create;
  $X['is_sequential'] = $is_sequential;
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
$X = set_is_sequential("asdsadasdasd", "ABTest", "rsubramonian", 1);
var_dump($X);
 */
?>

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

//<hdr>
//C \section{Set Follow On}
//C \label{set_follow_on}
//C Arguments are 
//C \begin{enumerate}
//C \item New Test 
//C \item Old Test 
//C \item Person requesting change
//C \end{enumerate}
function set_follow_on(
  $old_test_id, 
  $new_test, 
  $modifier
)
//</hdr>
{
  $GLOBALS["err"] = "";
  $dbh = dbconn(); 

  //C \begin{itemize}
  $xy_test_type_id = name_to_id("test_type", "name", "XYTest");
  $archived_id     = name_to_id("state", "name", "archived");
  $dormant_id      = name_to_id("state", "name", "dormant");
  //----------------------------------------------------------------
  //C \item Old Test must be 
  //C \begin{enumerate} 
  //C \item test Type = XYTest 
  //C \item State = Archived  
  //C \item Channel not null
  //C \end{enumerate}
  $T = db_get_row("abtest", "pk", $old_test_id);
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid OldTestID [$old_test_id] \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $old_pk           = $T['pk'];
  $old_external_id  = $T['external_id'];
  $old_channel_id   = $T['channel_id'];
  if ( $T['test_type_id']  != $xy_test_type_id ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  }
  if ( $T['state_id']  != $archived_id ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  }
  if ( !$T['channel_id'] ) {
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
  }
  //----------------------------------------------------------------
  //C \item Modifier must be valid amdin
  $A = db_get_row("admin", "name", $modifier);
  if (!$A ) { 
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $GLOBALS["err"] .= "Invalid admin [$modifier] "; return false;
  }
  //----------------------------------------------------
  //C \item New Test must be 
  //C \begin{enumerate} 
  //C \item test Type = XYTest 
  //C \item State = Dormant  
  //C \item Channel same as old test 
  //C \end{enumerate}
  $where_clause  = " and state_id = $dormant_id  and ";
  $where_clause .= " test_type_id = $xy_test_type_id and ";
  $where_clause .= " channel_id = $old_channel_id ";
  $T = db_get_row("abtest", "name", $new_test, $where_clause);
  if (!$T ) { 
    $GLOBALS["err"] .= "Invalid Test [$new_test] \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  $new_channel_id   = $T['channel_id'];
  $creator_id       = $T['creator_id'];
  $is_private       = $T['is_private'];
  $pk               = $T['pk'];
  //----------------------------------------------------
  //C \item If test is private, modifier must be creator 
  $modifier_id = $A['pk'];
  if ( ( $is_private == 1 ) && ( $modifier_id != $creator_id ) ) {
    $GLOBALS["err"] .= "User [$modifier] not creator\n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //----------------------------------------------------
  //C \item There should be no other XYTest with same external ID
  //C in non-archived state
  $where_clause  = " test_type_id = $xy_test_type_id and ";
  $where_clause .= " state_id != $archived_id and ";
  $where_clause .= " external_id = $old_external_id ";
  $nR = db_num_rows("abtest", $where_clause);
  if ( $nR > 0 ) { 
    $GLOBALS["err"] .= "Some other test is using this ID \n";
    $GLOBALS["err"] .= "ERR: FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    return false;
  }
  //----------------------------------------------------
  //C \end{itemize}
  $d_change = get_date(); 
  $t_change = get_time_usec(); 
  $sql  = "update abtest set ";
  $sql .= "external_id = :external_id, ";
  $sql .= "pred_id = :pred_id, ";
  $sql .= "most_recent_update = :d_create, ";
  $sql .= "t_update = :t_update where pk = :pk";
  $stmt = $dbh->prepare($sql);
  $X['external_id'] = $old_external_id;
  $X['pred_id']    = $old_pk;
  $X['d_create']   = $d_change;
  $X['t_update']   = $t_change;
  $X['pk']         = $pk;
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

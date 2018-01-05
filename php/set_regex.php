<?php
require_once "bye.php";
require_once "dbconn.php";
require_once "db_get_row.php";

function set_regex(
  $abtest,
  $modifier,
  $regex
)
{
  $status = true;
  $abtest = trim($abtest);
  $modifier = trim($modifier);
  $regex = trim($regex);

  if ( $regex == "" ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  // TODO: Make sure that regex contains only ascii characters
  // TODO: Why do we have this file at all if new_set_description does the 
  // trick?
  $dbh = dbconn(); 
  if ( !$dbh ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  //--------------------------------------------------------
  $A = db_get_row("admin", "name", $modifier);
  if ( !$A ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $modifier_id = $A['pk'];
  //--------------------------------------------------------
  $T = db_get_row("abtest", "name", $abtest, 
    " state_id = $dormant_id and test_type_id = $ab_test_type_id ");
  if ( !$T ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $abtest_id  = $T['pk'];
  $creator_id = $T['creator_id'];
  $is_private = $T['is_private'];
  if ( ( $is_private == 1 ) && ( $modifier_id != $creator_id ) ) { 
    $GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
    $status = false; goto BYE;
  }
  $rslt = db_set_row("abtest", "regex", $regex, " where pk = $abtest_id");
BYE:
  return $status;
}
?>

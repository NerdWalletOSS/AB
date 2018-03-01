<?php session_start();
# -- PARSE SESSION VARIABLES
if (isset($_SESSION['User'])) { $User = $_SESSION['User'];}
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/rts/");

require_once 'set_state.php';
require_once 'db_get_test.php';
//require_once '../create_good_json_test.php';
//-----------------------------------------------------------
//-------- ACCESS POST parameters

function action_state($state_id) {
  switch ($state_id) {
    case '1':
      return 'Publish';
    break;

    case '2':
      return 'Start';
    break;

    case '3':
      return 'Stop';
    break;

    case '4':
      return 'Archive';
    break;
  }
}

/*ob_start();
if ( !$_GET ) {
  echo '{ "Set State" : "ERROR", "Message" : "No paylaod" }'; exit;
}*/
//$str_inJ = json_encode($_POST);
$X = db_get_test($_GET['TestID']);
$X['NewState'] = action_state($_GET['state_id']);
$X['Updater'] = $User;
$str_inJ = json_encode($X);


//$json_input = create_good_json_test($str_inJ);
//if ( !$json_input ) {
//  echo '{ "InsertTest" : "ERROR", "Message" : "Bad JSOn" }'; exit;
//}
//-------------------------------------
// Call to set state
$outJ = set_state($str_inJ);
//var_dump($rslt);
//header("TestID: ".$rslt["TestID"]);
//ob_clean();
header('Location: ../home.php');

?>

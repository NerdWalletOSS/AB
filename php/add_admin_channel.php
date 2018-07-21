<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "../php/helpers/");
require_once "bye.php";
require_once "dbconn.php";
require_once "insert_row.php";
require_once "aux_chk_name.php";
require_once "db_get_row.php";
require_once "load_configs.php";
require_once "get_json_element.php";
require_once "lkp.php";
error_reporting(E_NOTICE);

function add_admin_channel(
  $str_inJ
)
{
  // START Check inputs
  rs_assert(!empty($str_inJ));
  rs_assert(is_string($str_inJ), "input not string");
  $inJ = json_decode($str_inJ); 
  rs_assert(gettype($inJ) != "string");
  rs_assert($inJ, "invalid JSON");

  $tbl  = get_json_element($inJ, 'Table'); 
  $tbl  = trim(strtolower($tbl));

  $name = get_json_element($inJ, 'Name'); 
  $name = trim(strtolower($name));

  //----------------------------------------------
  rs_assert( (($tbl == "admin" ) || ( $tbl == "channel" )), "table is $tbl");
  if ( $tbl == "admin" ) { 
    $max_len = lkp("configs", "max_len_admin_name");
  }
  else if ( $tbl == "channel" ) { 
    $max_len = lkp("configs", "max_len_channel_name");
  }
  rs_assert(aux_chk_name($name) );
  $name_len = strlen($name);
  // rs_sssert($name_len <= $max_len); TODO P4 PUT THIS BACK 
  // Check that entry does not exist
  $nR = db_get_row($tbl, "name", $name);
  rs_assert($nR == 0, "Entry $name in table $tbl exists");
  $X['name']   = $name;
  $X['is_del'] = 0;
  $rslt = insert_row($tbl, $X);
  rs_assert($rslt);

  $outJ['msg_stdout'] = "Added $name";
  $outJ['rts_code'] = 0;
  return $outJ;
}
/* For quick and dirty testing
$X = '{ "Table" : "admin", "Name" : "ramesh" } ';
var_dump($X);
$rslt = add_admin_channel($X);
var_dump($X);
var_dump($rslt);
 */
?>

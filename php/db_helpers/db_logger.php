<?php
set_include_path(get_include_path() . PATH_SEPARATOR . "../");
set_include_path(get_include_path() . PATH_SEPARATOR . "../helpers/");
require_once 'insert_row.php';
require_once 'get_time_usec.php';

function db_logger(
    $json_input, 
    $status,
    $api
  )
{
  $X['created_at']   = get_time_usec();
  $X['d_create']   = get_date();
  $X['msg']        = $json_input;
  $X['err_msg']    = $GLOBALS['err'];
  $X['is_success'] = $status;
  $X['api']        = trim($api);
  insert_row("log_ui_to_webapp", $X);
}
?>

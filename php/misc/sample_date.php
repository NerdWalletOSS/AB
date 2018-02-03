<?php

require_once 'dbconn.php';
require_once 'db_get_rows.php';

$d1 = new DateTime("now");
$d2 = date_sub($d1, date_interval_create_from_date_string('7 days'));
print_r($d2);
$d = $d2->date;
print_r($d);


$R = db_get_rows("log_ui_to_webapp", "d_create >= '$d' ");
$nR = count($R);
echo "\nGot $nR rows from log table\n";

?>


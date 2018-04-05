<?php

/*
echo "<br>===========<br>";
$x = file_get_contents('php://input');
var_dump($x);
echo "<br>===========<br>";
if (empty($_POST) ) {
  echo "<br> no post parameters<br";
}
else {
var_dump($_POST);
}
echo "<br>===========<br>";
$y = $_GET;
var_dump($y);
 */

header("Content-type: application/json; charset=utf-8");
$X['post'] = $_POST;
$X['get'] = $_GET;
$X['in'] = file_get_contents('php://input');
print("input = " . $X['in'] . " end input\n");
$Y = json_encode($X);
if ( !$Y ) { print("\nerror<br>\n"); }
else { print("\nsuccess<br>\n"); }
// print($Y);
print($X['in']);
?>

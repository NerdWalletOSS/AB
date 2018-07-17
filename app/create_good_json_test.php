<?php
require_once 'get_json_element.php';

function create_good_json_test($str_inJ) {

// START Check inputs
assert(!empty($str_inJ));
assert(is_string($str_inJ), "input not string");
$inJ = json_decode($str_inJ); assert($inJ, "invalid JSON");

// START CREATING DESIRED OUTPUT
$X = array();
$X['TestType'] = get_json_element($inJ, 'TestType'); 
$X['description'] = get_json_element($inJ, 'TestDescription');
if($X['TestType'] == "XYTest") { $X['Channel'] = get_json_element($inJ, 'Channel'); }
$X['id']  = get_json_element($inJ, 'TestID');
if ((isset($X['id'])) && ($X['id'] == "")) {
$X['Creator']  = get_json_element($inJ, 'Creator');
} else {
$X['State']  = get_json_element($inJ, 'State');
$X['Updater']  = get_json_element($inJ, 'Updater');
}
$X['name']  = get_json_element($inJ, 'TestName');
$X['BinType']  = get_json_element($inJ, 'BinType');

$n = get_json_element($inJ, 'NumVariants');
$V = array();
for ( $i = 0; $i < $n; $i++) {
if ($X['id'] != "") { $V[$i]['id'] = get_json_element($inJ, 'VID_'.$i.''); }
//if (isset($id)) {$V[$i]['id']  = get_json_element($inJ, 'VID_'.$i.'');}
$V[$i]['name']  = get_json_element($inJ, 'VName_'.$i.'');
$V[$i]['percentage']  = get_json_element($inJ, 'VPercentage_'.$i.'');
if($X['TestType'] == "XYTest") {
$url = get_json_element($inJ, 'VURL_'.$i.'');
if (isset($url) && ($url != "")) {$V[$i]['url']  = get_json_element($inJ, 'VURL_'.$i.'');
}
}
}

$X['Variants'] = $V;
$outJ = json_encode($X);
//echo $outJ;
return $outJ;

}

?>


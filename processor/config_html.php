<?php
function config_html($TestType) {

// START CREATING DESIRED OUTPUT
$X = array();
  if ($TestType == "ABTest") {
    $X['name_size'] = "63";
		$X['name_maxlength'] = "63";
		$X['desc_maxlength'] = "128";
  } else {
    $X['name_size'] = "127";
		$X['name_maxlength'] = "127";
  }

return $X;
}
?>

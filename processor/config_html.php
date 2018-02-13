<?php
function config_html($TestType) {

// START CREATING DESIRED OUTPUT
$X = array();
  if ($TestType == "ABTest") {
    $X['name_size'] = "63";
		$X['name_maxlength'] = "63";
		$X['desc_maxlength'] = "128";
  } elseif  ($TestType == "XYTest") {
    $X['name_size'] = "127";
		$X['name_maxlength'] = "127";
  }
		else {
  // throw error & die
  }

return $X;
}

?>

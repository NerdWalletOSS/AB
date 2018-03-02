<?php
function is_valid_json($string)
{
	$GLOBALS['err'] = "";
    // decode the JSON data
    $result = json_decode($string);

    // switch and check possible JSON errors
    switch (json_last_error()) {
        case JSON_ERROR_NONE:
            $GLOBALS["err"] .= ''; // JSON is valid // No error has occurred
			#$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        case JSON_ERROR_DEPTH:
            $GLOBALS["err"] .= 'The maximum stack depth has been exceeded.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        case JSON_ERROR_STATE_MISMATCH:
            $GLOBALS["err"] .= 'Invalid or malformed JSON.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        case JSON_ERROR_CTRL_CHAR:
            $GLOBALS["err"] .= 'Control character error, possibly incorrectly encoded.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        case JSON_ERROR_SYNTAX:
            $GLOBALS["err"] .= 'Syntax error, malformed JSON.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        // PHP >= 5.3.3
        case JSON_ERROR_UTF8:
            $GLOBALS["err"] .= 'Malformed UTF-8 characters, possibly incorrectly encoded.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        // PHP >= 5.5.0
        case JSON_ERROR_RECURSION:
            $GLOBALS["err"] .= 'One or more recursive references in the value to be encoded.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        // PHP >= 5.5.0
        case JSON_ERROR_INF_OR_NAN:
            $GLOBALS["err"] .= 'One or more NAN or INF values in the value to be encoded.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        case JSON_ERROR_UNSUPPORTED_TYPE:
            $GLOBALS["err"] .= 'A value of a type that cannot be encoded was given.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
        default:
            $error = 'Unknown JSON error occured.';
			$GLOBALS["err"] .= "FILE: " . __FILE__ . " :LINE: " . __LINE__ . "\n";
            break;
    }

    if (json_last_error() != 'JSON_ERROR_NONE') {
        // throw the Exception or exit // or whatever :)
    return false;
    }

    // everything is OK
    return true;
}

/*
$j = '[{"user_id":13,"username":"stack"},{"user_id":14,"username":"over"}]';
$rslt = is_valid_json($j);
var_dump($rslt);
var_dump($GLOBALS['err']);
*/

?>
<?php
function make_boolean(
  $in_str
)
{
  if ( !isset($in_str) || ( $in_str == "" ) || 
    ( strtolower($in_str) == "false" ) ) {
      return false;
    }
  else if ( strtolower($in_str) == "true" ) {
    return true;
  }
  else {
    return null;
  }
}

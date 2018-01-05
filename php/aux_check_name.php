<?php
function aux_check_name(
    $name
)
{
  if ( ( is_null($name) ) || ( $name == "" ) ) {
    return false;
  }
  $X = str_split($name);
  for ( $i = 0; $i < count($X); $i++ ) { 
    if ( ( ctype_alnum($X[$i]) ) || ( $X[$i] == "_" ) )  {
      // All is well 
    }
    else {
      $GLOBALS["err"] .= "See position $i of [$name]";
      return false;
    }
  }
  return true;
}
function chk_url_text(
    $name
)
{
  $valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";
  if ( ( is_null($name) ) || ( $name == "" ) ) {
    return false;
  }
  $X = str_split($name);
  for ( $i = 0; $i < count($X); $i++ ) { 
    if ( !strstr($valid_url_chars, $X[$i]) ) {
      return false;
    }
  }
  return true;
}
?>

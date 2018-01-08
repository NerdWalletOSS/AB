<?php
function aux_chk_name(
    $name
)
{
  if ( !isset($name) ) { return false; }
  if ( !is_string($name)) { return false; }
  if ( $name == "" ) { return false; }
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
    $url
)
{
  if ( !isset($url) ) { return false; }
  if ( !is_string($url)) { return false; }
  if ( $url == "" ) { return false; }
  $valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";
  $X = str_split($url);
  for ( $i = 0; $i < count($X); $i++ ) { 
    if ( !strstr($valid_url_chars, $X[$i]) ) {
      return false;
    }
  }
  return true;
}
?>

<?php
function is_new_test($inJ)
{
  if ( ( isset($inJ->{'id'} )  && ($inJ->{'id'} == "" ) ) ||
    ( !isset($inJ->{'id'}) ) ) {
      return true;
    }
  else {
    return false;
  }
}
?>

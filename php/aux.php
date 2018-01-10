<?php
function get_json(
  $J,
  $key
)
{
  assert(isset($J));
  assert(isset($key));
  assert(isset($J->{$key}));
  if ( is_string($J->{$key}) )  {
    return trim($J->{$key});
  }
  else {
    return $J->{$key};
  }
}

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

function make_pos_int(
  $in_str
)
{
  assert( ( isset($in_str) ) && ( $in_str != "" ) );
  $outval = intval($in_str);
  assert($outval > 0 );
  return $outval;
}
function is_unique(
  $X
)
{
  assert(isset($X));
  assert(is_array($X));
  assert(count($X) > 0 );
  for ( $i = 0; $i < count($X); $i++ ) { 
    for ( $j = $i+1; $j < count($X); $j++ ) { 
      assert($X[$i] != $X[$j]);
    }
  }
  return true;
}
function is_good_percs(
  $X
)
{
  assert(isset($X));
  assert(is_array($X));
  assert(count($X) > 0 );
  $nX = count($X);
  assert($nX >= lkp('configs', "min_num_variants"));
  assert($nX <= lkp('configs', "max_num_variants"));
  foreach ( $X as $p ) {
    assert(is_float($p), "percentage must be a number");
    assert($p >=   0, "percentage must not be negative");
    assert($p <= 100, "percentage cannot exceed 100");
  }
  $sum = 0;
  for ( $i = 0; $i < count($X); $i++ ) { 
    assert(is_float($X[$i]));
    assert($X[$i] >= 0 );
    assert($X[$i] <= 100 );
    $sum += $X[$i];
  }
  assert($sum = 100);
  return true;
}
function is_good_variants(
  $V
)
{
  assert(isset($V));
  assert(is_array($V));
  assert(count($V) > 0 );
  $nV = count($V);
  assert($nV >= lkp('configs', "min_num_variants"));
  assert($nV <= lkp('configs', "max_num_variants"));
  assert(is_unique($V));
  foreach ( $V as $v ) {
    assert(aux_chk_name($v), "variant name is invalid");
    assert(strlen($v) <= lkp("configs", "max_len_variant_name"));
  }
}
function is_good_test_name(
  $test_name, 
  $test_type,
  $test_id = -1
)
{
  assert(aux_chk_name($test_name), "test name is invalid");
  assert(strlen($test_name) <= lkp("configs", "max_len_test_name"));
  assert(is_test_name_unique($test_name, $test_type, $test_id), 
  "test name [$test_name] not unique");
  return true;
}
function is_test_name_unique(
  $test_name,
  $test_type
)
{
  $test_type_id = lkp("test_type", $test_type);
  $archived_id  = lkp("state", "archived");
  $where_clause  = " name = '$test_name' and ";
  $where_clause .= " state_id != $archived_id and ";
  $where_clause .= " test_type_id = $test_type_id ";
  $R = db_get_rows("test", $where_clause);
  if ( is_null($R)) {
    return true;
  }
  else {
    return false;
  }
}
?>

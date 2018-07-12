local function compare_T(
  T1,
  T2
  )
  print(T1)
  print(T2)
  assert(type(T1) == "table")
  assert(type(T2) == "table")
  assert(T1 ~= T2 )
  --==================================
  assert(T1.name)
  assert(T1.id)
  assert(T1.external_id)
  assert(T1.TestType)
  assert(T1.State)
  assert(T1.is_dev_specific)
  assert(T1.seed)
  --==================================
  assert(T1.name            == T2.name )
  assert(T1.id              == T2.id )
  assert(T1.TestType        == T2.TestType )
  assert(T1.State           == T2.State )
  assert(T1.is_dev_specific == T2.is_dev_specific )
  assert(T1.external_id     == T2.external_id    )
  assert(T1.seed            == T2.seed )
  if ( ( T1.NumVariants ) and ( T2.NumVariants ) ) then 
    assert(T1.NumVariants ==  T2.NumVariants)
  end
  --==================================
  local V1 = assert(T1.Variants)
  local V2 = assert(T2.Variants)
  assert(#V1 == #V2 ) 
  for _, v1 in pairs(V1) do
    local found = false
    for _, v2 in pairs(V2) do
      if ( v1.name == v2.name ) then
        assert( v1.id == v2.id )
        assert( v1.percentage == v2.percentage )
        if ( T1.TestType == "XYTest" ) then 
          assert(v1.url)
          assert(v1.url == v2.url)
        end
        if ( T1.TestType == "ABTest" ) then 
          if ( ( v1.custom_data ) or ( v2.custom_data ) ) then 
            assert(v1.custom_data == v2.custom_data)
          end
        end
        found = true; break
      end
    end
    assert(found)
  end
  return true
end
return compare_T

local function random_perc(T)
  local TestType = assert(T.TestType)
  local Variants = assert(T.Variants)
  local nV = #Variants
  assert(nV >= 2)
  if ( TestType == "XYTest" ) then
    local sum_perc = 0
    local balance = 100
    for i = 1, nV-1 do 
      if ( balance == 0 ) then 
        T.Variants[i].percentage = 0
      else
        local perc = math.random(0, balance)
        T.Variants[i].percentage = perc
        balance = balance - perc
      end
    end
    -- last guy gets whateveris left over
    T.Variants[nV].percentage = balance
  elseif ( TestType == "ABTest" ) then
    print("TODO: no change to perc as yet")
  else
    assert(nil)
  end
  return T
end
return random_perc

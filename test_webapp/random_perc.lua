local function random_perc(T)
  local TestType = assert(T.TestType)
  local Variants = assert(T.Variants)
  local nV = #Variants
  local perc = {}
  assert(nV >= 2)
  if ( TestType == "XYTest" ) then
    local sum_perc = 0
    local balance = 100
    local perc
    for i = 1, nV-1 do 
      if ( balance == 0 ) then 
        perc = 0
      else
        perc = math.random(0, balance)
        balance = balance - perc
      end
      perc[T.Variants[i].name] = perc
    end
    -- last guy gets whateveris left over
    perc[T.Variants[nV].name] = balance
  elseif ( TestType == "ABTest" ) then
    print("TODO: no change to perc as yet")
  else
    assert(nil)
  end
  return perc
end
return random_perc


local function classify(
  T, -- table of m lvectors of length n
  g, -- lVector of length n
  x, -- table of m Scalars 
  alpha -- table of m Scalars (scale for different attributes)
  )
  -- START: Checking
  local nT = 0
  local nx = 0
  local nalpha = 0
  local n
  --=====================================
  assert(type(T) == "table")
  for k, v in pairs(T) do 
    assert(type(v) == "lVector")
    assert(v:fldtype() == "F4")
    if ( nT == 1 ) then 
      n = v:length()
    else
      assert(n == v:length())
    end
    nT = nT + 1
  end
  assert(#T = nT)
  --=====================================
  assert(type(alpha) == "table")
  for k, v in pairs(T) do 
    assert(type(v) == "Scalar")
    assert(v:fldtype() == "F4")
    nalpha = nalpha + 1
  end
  assert(#alpha == nalpha)
  --=====================================
  assert(type(x) == "table")
  for k, v in pairs(T) do 
    assert(type(v) == "Scalar")
    assert(v:fldtype() == "F4")
    nx = nx + 1
  end
  assert(#x == nx)
  --=====================================
  assert(nx == nT)
  assert(nalpha == nx)
  --=====================================
  g:length() == n
  g:fldtype() == "I4"
  local minval, numval = Q.min(g):eval()
  assert(minval == Scalar.new(0, "I4"))
  local maxval, numval = Q.max(g):eval()
  local ng = maxval - minval + 1 -- number of values of goal attr
  --=====================================
  -- STOP : Checking
  dk = {}
  for i = 1, nx do 
    dk[i] = Q.vsmul(Q.sqr(Q.vssub(T[i], x[i])), alpha[i])
  end
  local one = Scalar.new(0, "F4")
  local d = Q.const({ val = one, qtype = "F4", len = n})
  for i = 1, nx do 
    d = Q.vvsum(dk[i], d):eval()
  end
  d = Q.reciprocal(d):eval()
  -- Now, we need to sum d grouped by value of goal attribute
  local rslt = Q.sumby(d, g, ng)
  return rslt 
end

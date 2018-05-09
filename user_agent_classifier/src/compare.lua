plpath = require 'pl.path'

actual_file = "_output_1.csv"
expected_file = "../data/output_1.csv"
assert(plpath.isfile(actual_file))
assert(plpath.isfile(expected_file))
--========================
local file = io.open(actual_file, "r");
local actual = {}
for line in file:lines() do
  table.insert (actual, line);
end

--========================
local file = io.open(expected_file, "r");
local expected = {}
for line in file:lines() do
  table.insert (expected, line);
end
n = #actual
assert(#actual == #expected)
--===================================
local num_correct = 0
local crosstab = {}
for i = 1, #actual do 
  if ( actual[i] == expected[i] ) then 
    num_correct = num_correct + 1
  else
    local jointkey = actual[i] .. "," .. expected[i]
    if ( crosstab[jointkey] ) then 
      crosstab[jointkey] = crosstab[jointkey] + 1
    else
      crosstab[jointkey] = 1
    end
  end
end
for k, v in pairs(crosstab) do 
  print(k .. "," .. v) 
end
print(num_correct .. " correct out of " .. n)

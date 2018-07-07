plfile = require 'pl.file'
plpath = require 'pl.path'

filename = '../../../src/ab_log_globals.h'
assert(plpath.isfile(filename))
keep = false
n = 1
metric = {}
comment = {}
for line in io.lines(filename) do
  if ( string.find(line, "EXTERN" ) ) then
    keep = true
    line = string.gsub(line, ";", "+ & ")
    line = string.gsub(line, "EXTERN uint64_t ", "\\verb+")
    metric[n] = line
  end
  if ( string.find(line, "/*C " ) ) then
    if ( keep ) then 
      line = string.gsub(line, "/%*C ", " ")
      line = string.gsub(line, "*/", "\\\\ \\hline")
      comment[n] = line
      n = n+1
      keep = false
    end
  end
end
for i = 1, n-1 do
  print(metric[i], comment[i])
end

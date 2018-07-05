plfile = require 'pl.file'
plpath = require 'pl.path'

filename = '../../../sql/create_table.sql'
assert(plpath.isfile(filename))
for line in io.lines(filename) do
  if ( string.match(line, "DROP TABLE" ) ) then
    line = string.gsub(line, "DROP TABLE IF EXISTS", "\\verb+")
    line = string.gsub(line, ";", "+ & ")
    line = string.gsub(line, "%-%-", "")
    line = string.gsub(line, "$", " \\\\ \\hline")
    print(line)
  end
end

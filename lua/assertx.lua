local function assertx(a, ...)
  if a then return a, ... end
  if ... ~= nil  then
    local args = {...}
    error(table.concat(args), 2)
  else
    error("assertion failed!", 2)
  end
end

return assertx

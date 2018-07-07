local x = {}
local max_len_variant_dscr = 255
for i = 1, max_len_variant_dscr + 1 do
  x[#x+1] = 'a'
end
local y = table.concat(x)

return {  description = y,
          custom_data = ' { "foo" : "bar" } ',
          url = "^!#*9**&$&*!#&*^&*!(){}:\"<>?/.,",
          -- INVALID CHARACTERS IN URL
        }

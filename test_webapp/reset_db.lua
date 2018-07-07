require 'lua/strict'
local function reset_db()
  local command = [[
    cd ../sql/ 1>/dev/null 2>&1; 
    bash reset_db.sh; 
    cd - 1>/dev/null 2>&1
    ]]
  local status = os.execute(command)
  assert(status == 0)
end
return reset_db

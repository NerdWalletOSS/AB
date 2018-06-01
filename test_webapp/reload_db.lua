require 'strict'
local plpath = require 'pl.path'
local function reload_db(
  sqldump_file
  )
  assert(plpath.isfile(sqldump_file))
  local command = [[
    cd ../sql/ 1>/dev/null 2>&1; 
    bash reset_db.sh; 
    mysql -u root --database abdb2 < __SQLDUMP_FILE__
    cd - 1>/dev/null 2>&1
    ]]
    print(sqldump_file)
    string.gsub(command, "__SQLDUMP_FILE__", sqldump_file)
    print(command)
  local status = os.execute(command)
  assert(status == 0)
end
return reload_db

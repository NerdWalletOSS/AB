local header_files = {}

header_files[#header_files+1] = "../dt_interpreter/src/dt_types.h"
header_files[#header_files+1] = "../dt_server/load_models.h"
header_files[#header_files+1] = "../dt_server/free_interp.h"
header_files[#header_files+1] = "../dt_server/classify.h"

return header_files

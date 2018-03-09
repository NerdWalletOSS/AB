#include "dt_incs.h"
#include "file_exists.h"

//START_FUNC_DECL
bool 
file_exists (
    const char * const filename
    )
//STOP_FUNC_DECL
{
  struct stat buf;
  if ( ( filename == NULL ) || ( *filename == '\0' ) ) { return false; }
  int status = stat(filename, &buf );
  if ( status == 0 ) { /* File found */
    return true;
  }
  else {
    return false;
  }
}


#include "dt_incs.h"
#include "auxil.h"
#include "dt_auxil.h"
#include "mmap.h"
extern char g_rslt[DT_MAX_LEN_RESULT+1]; // For C: ab_process_req()

int
add_to_buf(
    char *in,
    const char *label,
    char *out,
    int sz_out,
    int *ptr_n_out
    )
{
  int status = 0;
  int n_out = *ptr_n_out;
  
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '"';
  if ( n_out + (int)strlen(label) >= sz_out ) { go_BYE(-1); }
  strcpy(out+n_out, label); n_out += strlen(label);
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '"';
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = ' ';
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = ':';
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = ' ';
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '"';

  for ( char *cptr = in; *cptr != '\0'; cptr++ ) { 
    if ( n_out >= sz_out ) { go_BYE(-1); }
    if ( isspace(*cptr) ) { g_rslt[n_out++] = ' '; continue; }
    if ( ( *cptr == '"' ) || ( *cptr == '\\' ) ) { 
      g_rslt[n_out++] = '\\';  
    }
    g_rslt[n_out++] = *cptr;
  }
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '"';
  *ptr_n_out = n_out;
BYE:
  return status;
}
  //--------------------------------------
int
mk_json_output(
    char *api, 
    char *args, 
    char *err, 
    char *out
    )
{
  int status = 0;
  memset(g_rslt, '\0', DT_MAX_LEN_RESULT+1);
  int n_out = 0; int sz_out = DT_MAX_LEN_RESULT;
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '{';
  status = add_to_buf(api, "API", g_rslt, sz_out, &n_out); cBYE(status);
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = ',';
  status = add_to_buf(args, "ARGS", g_rslt, sz_out, &n_out); cBYE(status);
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = ',';
  status = add_to_buf(err, "ERROR", g_rslt, sz_out, &n_out); cBYE(status);
  if ( n_out >= sz_out ) { go_BYE(-1); } out[n_out++] = '}';
  //--------------------------------------
BYE:
  return status;
}

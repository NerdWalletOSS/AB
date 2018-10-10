#include "dt_incs.h"
#include "auxil.h"
#include "dt_auxil.h"
#include "mmap.h"

#ifdef XXXX
//<hdr>
int 
chk_test_name(
    const char *X
    )
//</hdr>
{
  int status = 0;

  if ( ( X == NULL ) || ( *X == '\0' ) )  { 
    g_log_bad_test_name++; STATSD_COUNT("bad_test_name", 1); go_BYE(-1); 
  }
  if ( strlen(X) > AB_MAX_LEN_TEST_NAME ) { 
    g_log_bad_test_name++; STATSD_COUNT("bad_test_name", 1); go_BYE(-1); 
  }
  for ( const char *cptr = X; *cptr != '\0'; cptr++ ) {
    if ( ( !isalnum(*cptr) ) && ( *cptr != '_' ) ) { 
      g_log_bad_test_name++; STATSD_COUNT("bad_test_name", 1); go_BYE(-1); 
    }
  }
BYE:
  return status;
}
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
  memset(g_rslt, '\0', AB_MAX_LEN_RESULT+1);
  int n_out = 0; int sz_out = AB_MAX_LEN_RESULT;
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
#endif

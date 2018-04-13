#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include <strings.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "ab_auxil.h"
#include "mmap.h"

//<hdr>
int 
chk_test_name(
    const char *X
    )
//</hdr>
{
  int status = 0;

  if ( ( X == NULL ) || ( *X == '\0' ) )  { 
    g_log_bad_test_name++; go_BYE(-1); 
  }
  if ( strlen(X) > AB_MAX_LEN_TEST_NAME ) { 
    g_log_bad_test_name++; go_BYE(-1); 
  }
  for ( const char *cptr = X; *cptr != '\0'; cptr++ ) {
    if ( ( !isalnum(*cptr) ) && ( *cptr != '_' ) ) { 
      g_log_bad_test_name++; go_BYE(-1); 
    }
  }
BYE:
  return status;
}

// This is very specific to NerdWallet 
int
chk_uuid(
    char *X,
    int desired_len
    )
{
  int status = 0;
  if ( ( X == NULL ) || ( *X == '\0' ) )  { 
    g_log_bad_uuid++;
    go_BYE(-1); 
  }
  for ( int i = 0; ; i++ ) { 
    char c = X[i];
    if ( c == '\0' ) { break; }
    if ( i+1 > desired_len ) { go_BYE(-1);}
    if ( c == '-' ) { continue; } // hyphen is allowed 
    if ( !isalnum(c ) ) { 
      g_log_bad_uuid++; go_BYE(-1); 
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

void
get_tracer(
      const char *args, 
      char *tracer
      )
{
  int status = 0;
  memset(tracer, '\0', AB_MAX_LEN_TRACER+1);
  status = extract_name_value(args, "Tracer=", '&', tracer, 
      AB_MAX_LEN_TRACER); 
  if ( ( status < 0 ) || ( *tracer ==  '\0' ) ) { 
    g_log_no_tracer++;
  }
  else {
    if ( !chk_tracer(tracer, AB_MAX_LEN_TRACER) ) { 
      g_log_bad_tracer++;
    }
  }
}

void
set_tracer(
    char *tracer,
    size_t n
    )
{
  memset(tracer, '\0', n+1);
  // TODO TO BE IMPLEMENTED 
}

#include "yuarel.h"
int 
get_host(
    char *in,
    char *out,
    int n_out
    )
{
  int status = 0;
  struct yuarel url;
  if ( in == NULL ) { go_BYE(-1); }
  if ( out == NULL ) { go_BYE(-1); }
  if ( n_out == 0 ) { go_BYE(-1); }
  memset(out, '\0', n_out=1); // NOTICE THE +1. Caller should take care
  status = yuarel_parse(&url, in); cBYE(status);
  strncpy(out, url.host, n_out);

BYE:
  return status;
}

#include "regdom.h"
int 
get_domain(
    const char *in,
    char *out,
    int n_out
    )
{
  int status = 0;
  void *tree = NULL;
  char *result = NULL;
  tree = loadTldTree();

  result = getRegisteredDomain(in, tree);
  memset(out, '\0', n_out+1); // NOTICE THE +1. Caller should take care
  if ( result == NULL ) { go_BYE(-1); }
  strncpy(out, result, n_out);
BYE:
  if ( tree != NULL ) { freeTldTree(tree); tree = NULL; }
  return status;
}

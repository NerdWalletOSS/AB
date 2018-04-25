#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "spooky_hash.h"
#include "get_test_idx.h"

#include "ab_globals.h"

int chk_if_dupe(
    const char *name,
    bool *ptr_is_dupe
    )
{
  int status = 0;
  *ptr_is_dupe = false;
  if ( ( name == NULL ) || ( *name == '\0' ) ) { go_BYE(-1); }
  uint64_t name_hash = spooky_hash64(name, strlen(name), g_seed1);
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    if ( name_hash == g_tests[i].name_hash ) {
      if ( strcmp(g_tests[i].name, name) == 0 ) { 
        *ptr_is_dupe = true; break; 
      }
    }
  }
BYE:
   return status;
}

int get_test_idx(
    const char *name,
    int test_type,
    int *ptr_test_idx
    )
{
  int status = 0;
  int num_probes = 0;
  *ptr_test_idx   = -1; 
  if ( ( name == NULL ) || ( *name == '\0' ) ) { go_BYE(-1); }
  uint64_t name_hash = spooky_hash64(name, strlen(name), g_seed1);
  int start = name_hash % AB_MAX_NUM_TESTS;
  for ( int i = start; i < AB_MAX_NUM_TESTS; i++ ) {
    num_probes++;
    if ( name_hash == g_tests[i].name_hash ) {
      if ( strcmp(g_tests[i].name, name) == 0 ) { 
        if ( g_tests[i].test_type == test_type ) { 
          *ptr_test_idx = i; 
          break;
        }
      }
    }
  }
  if ( *ptr_test_idx < 0 ) { 
    for ( int i = 0; i < start; i++ ) {
      num_probes++;
      if ( name_hash == g_tests[i].name_hash ) {
        if ( strcmp(g_tests[i].name, name) == 0 ) { 
          if ( g_tests[i].test_type == test_type ) { 
            *ptr_test_idx = i; break;
          }
        }
      }
    }
  }
  if ( *ptr_test_idx < 0 ) { 
    g_log_missing_test++;
    go_BYE(-1);
  }
  /* TODO: statsd logging for num_probes */
BYE:
  return status;
}


int get_state_enum(
    const char *state,
    int *ptr_state_enum
    )
{
  int status = 0;
  if ( ( state == NULL ) || ( *state == '\0' ) )  { go_BYE(-1); }
  if ( strcasecmp(state, "started") == 0 ) {
    *ptr_state_enum = TEST_STATE_STARTED;
  }
  else if ( strcasecmp(state, "terminat ed") == 0 ) {
    *ptr_state_enum = TEST_STATE_TERMINATED;
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}
//<hdr>
int
get_test_type(
      const char *args, 
      int *ptr_test_type
      )
//</hdr>
{
  int status = 0;
  char str_test_type[AB_MAX_LEN_TEST_TYPE+1];
  /* Extract TestType  */
  *ptr_test_type = -1;
  memset(str_test_type, '\0', AB_MAX_LEN_TEST_TYPE+1);
  status = extract_name_value(args, "TestType=", '&', str_test_type, 
      AB_MAX_LEN_TEST_TYPE);
  cBYE(status);
  if ( strcasecmp(str_test_type, "ABTest") == 0 ) { 
    *ptr_test_type = AB_TEST_TYPE_AB;
  }
  else if ( strcasecmp(str_test_type, "XYTest") == 0 ) { 
    *ptr_test_type = AB_TEST_TYPE_XY;
  }
  else { 
    g_log_bad_test_type++;
    go_BYE(-1); 
  }
BYE:
  return status;
}

//<hdr>
int 
get_test_name(
    const char *args, 
    char *test_name
    )
{
  int status = 0;
  /* Extract test name */
  memset(test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = extract_name_value(args, "TestName=", '&', 
      test_name, AB_MAX_LEN_TEST_NAME);
  if ( status < 0 ) { g_log_no_test_name++; }
  cBYE(status); // ADDED DEC 2016
  status = chk_test_name(test_name); 
  if ( status < 0 ) { g_log_bad_test_name++; }
  cBYE(status);
BYE:
  return status;
}

int
get_test_from_args(
    const char *args,
    int *ptr_test_idx,
    int *ptr_test_type
    )
{
  int status = 0;
  int bufsz = 511; int test_type, test_idx;
  char test_name[AB_MAX_LEN_TEST_NAME+1]; 
  char *buf = NULL;

  buf = malloc(bufsz+1);
  memset(buf, '\0',  bufsz+1);
  //-----------------------------------------------
  memset(buf, '\0', bufsz+1);
  status = extract_name_value(args, "TestType=", '&', buf, bufsz);
  cBYE(status);
  if ( buf[0] == '\0' ) { go_BYE(-1); }
  if ( strcmp(buf, "ABTest") == 0 ) { 
    test_type = AB_TEST_TYPE_AB;
  }
  else if ( strcmp(buf, "XYTest") == 0 ) { 
    test_type = AB_TEST_TYPE_XY;
  }
  else {
    go_BYE(-1);
  }
  //-----------------------------------------------
  memset(test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = extract_name_value(args, "TestName=", '&', test_name, 
      AB_MAX_LEN_TEST_NAME);
  cBYE(status);
  if ( test_name[0] == '\0' ) { go_BYE(-1); }
  //-----------------------------------------------
  //-- Test must exist
  status = get_test_idx(test_name, test_type, &test_idx); 
  if ( ( status != 0 ) || ( test_idx < 0 ) )  { go_BYE(-1); }
  *ptr_test_idx = test_idx;
  *ptr_test_type = test_type;
BYE:
  free_if_non_null(buf);
  return status;
}

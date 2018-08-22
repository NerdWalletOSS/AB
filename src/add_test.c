#include "ab_incs.h"
#include "auxil.h"
#include "aux_zero.h"
#include "add_test.h"
#include "spooky_hash.h"
#include <jansson.h>
#include <lua.h>
#include "read_conf_file.h"
#include "get_test_idx.h"
#include "add_fake_test.h"

extern uint64_t g_seed1;
extern lua_State *g_L;
extern TEST_META_TYPE g_tests[AB_MAX_NUM_TESTS];
extern char g_rslt[AB_MAX_LEN_RESULT+1];
extern char g_err[AB_MAX_LEN_RESULT+1];
extern int g_n_justin_cat_lkp;
// int cdata[1];
int
add_test(
    const char *args
    )
{
  int status = 0;
  int itemp, state, test_type, test_idx = -1;
  char test_name[AB_MAX_LEN_TEST_NAME+1];
  int is_dev_specific; 
  int num_variants;
#define BUFLEN 63
  char cbuf[BUFLEN+1];
  // read buffer into JSON
  json_t *root = NULL;
  json_error_t error;
  root = json_loads(args, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }
  //------------------------------------------
  status = get_int(root, "NumVariants", NULL, NULL, &num_variants); 
  cBYE(status);
  if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
      ( num_variants > AB_MAX_NUM_VARIANTS ) ) { 
    go_BYE(-1);
  }
  //----------------------------------------
  memset(cbuf, '\0', BUFLEN+1);
  status = get_string(root, "TestType", NULL, NULL, 16, cbuf); cBYE(status);
  status = get_test_type_enum(cbuf, &test_type); cBYE(status);
  //------------------------------------------
  memset( test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = get_string(root, "name", NULL, NULL, AB_MAX_LEN_TEST_NAME, 
      test_name); cBYE(status);
  //------------------------------------------
  memset(cbuf, '\0', BUFLEN);
  status = get_string(root, "State", NULL, NULL, 16, cbuf); cBYE(status);
  status = get_state_enum(cbuf, &state); cBYE(status);
  if ( state == TEST_STATE_ARCHIVED ) {  // delete and get out of here
    status = get_test_idx(test_name, test_type, &test_idx); 
    if ( test_idx < 0 ) { go_BYE(-1); }
    free_test(test_idx); goto BYE; 
  }
  //------------------------------------------
  status = get_int(root, "is_dev_specific", NULL, NULL, &is_dev_specific); cBYE(status);
  if ( ( is_dev_specific < 0 )  || ( is_dev_specific > 1 ) ) { go_BYE(-1); }
  //----------------------------------------
  status = get_test_idx(test_name, test_type, &test_idx); 
  if ( test_idx < 0 ) { // seeing this for the first time
    status = get_empty_spot(test_name, &test_idx); cBYE(status);
    if ( test_idx < 0 ) { go_BYE(-1); }
    status = malloc_test(test_idx, num_variants, is_dev_specific, state);
    cBYE(status);
    if ( state == TEST_STATE_TERMINATED ) {
      status = free_variant_per_bin(test_idx); cBYE(status);
      status = malloc_final_variant(test_idx); cBYE(status);
    }
    // update test  --- see below
  }
  else { // updating an existing test 
    if ( state == TEST_STATE_TERMINATED ) {
      if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
        /* nothing to do */
        goto BYE;
      }
      else if ( g_tests[test_idx].state == TEST_STATE_STARTED ) {
        status = free_variant_per_bin(test_idx); cBYE(status);
        status = malloc_final_variant(test_idx); cBYE(status);
        // update test --- see below
      }
    } 
    else if ( state == TEST_STATE_STARTED ) {
      if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
        go_BYE(-1); 
      }
      else if ( g_tests[test_idx].state == TEST_STATE_STARTED ) {
        // update test --- see below
      }
    }
  }
  // following set in malloc
  if ( g_tests[test_idx].num_devices <= 0 ) { go_BYE(-1); }
  // Part of the update done on the C side 
  strncpy(g_tests[test_idx].name, test_name, AB_MAX_LEN_TEST_NAME);
  g_tests[test_idx].test_type = test_type;
  g_tests[test_idx].state     = state;
  g_tests[test_idx].num_variants     = num_variants;
  //----------------------
  memset(cbuf, '\0', BUFLEN+1);
  char *endptr;
  status = get_string(root, "external_id", NULL, NULL, 32, cbuf); 
  if ( *cbuf == '\0' ) { 
    g_tests[test_idx].external_id = 0; 
  }
  else {
    g_tests[test_idx].external_id = strtoull(cbuf, &endptr, 10);
  }
  //----------------------
  memset(cbuf, '\0', BUFLEN+1);
  status = get_string(root, "seed", NULL, NULL, 32, cbuf); cBYE(status);
  if ( *cbuf == '\0' ) { cBYE(status); }
  g_tests[test_idx].seed = strtoull(cbuf, &endptr, 10);
  //----------------------
  g_tests[test_idx].is_dev_specific  = is_dev_specific;
  g_tests[test_idx].name_hash = 
    spooky_hash64(test_name, strlen(test_name), g_seed1);
  int len = strlen(args)+1;
  g_tests[test_idx].test_as_str = malloc(len);
  memset(g_tests[test_idx].test_as_str, '\0', len);
  strcpy(g_tests[test_idx].test_as_str, args);
  // Part of the update done on the Lua side 
  cBYE(status);
  if ( g_L == NULL ) { go_BYE(-1); }
  lua_getglobal(g_L, "update_test");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Lua function update_test not found\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L, &(g_tests[test_idx]));
  lua_pushstring(g_L, args);
  status = lua_pcall(g_L, 2, 0, 0);
  if ( status != 0) {
    fprintf(stderr, "Lua function update_test failed: %s\n", 
        lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1); go_BYE(-1);
  }

  json_decref(root);
BYE:
  if ( ( status < 0 ) && ( test_idx >= 0 ) ) { 
    free_test(test_idx);
  }
  return status;
}

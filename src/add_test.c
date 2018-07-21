#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "aux_zero.h"
#include "add_test.h"
#include <jansson.h>
#include "read_conf_file.h"
#include "get_test_idx.h"
#include "add_fake_test.h"

// int cdata[1];
int
add_test(
    const char *args
    )
{
  int status = 0;
  int itemp, state, test_type, test_idx;
  char test_name[AB_MAX_LEN_TEST_NAME+1];
  int is_dev_specific; 
  int num_variants;
#define BUFLEN 32768
  char cbuf[BUFLEN];
  // read buffer into JSON
  json_t *root = NULL;
  json_error_t error;
  root = json_loads(args, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }
  //------------------------------------------
  status = get_int(root, "id", NULL, NULL, &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  //----------------------------------------
  fprintf(stderr, "\n%s\n", args);
  status = get_int(root, "NumVariants", NULL, NULL, &num_variants); 
  cBYE(status);
  if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
       ( num_variants > AB_MAX_NUM_VARIANTS ) ) { 
    go_BYE(-1);
  }
  //----------------------------------------
  memset(cbuf, '\0', BUFLEN);
  status = get_string(root, "State", NULL, NULL, 16, cbuf); cBYE(status);
  status = get_state_enum(cbuf, &state); cBYE(status);
  if ( state == TEST_STATE_ARCHIVED ) {  // delete and get out of here
    test_idx = 0; // TODO FIX P0
    if ( test_idx < 0 ) { go_BYE(-1); }
    free_test(test_idx);
    goto BYE; 
  }
  //------------------------------------------
  memset(cbuf, '\0', BUFLEN);
  status = get_string(root, "TestType", NULL, NULL, 16, cbuf); cBYE(status);
  status = get_test_type_enum(cbuf, &test_type); cBYE(status);
  //------------------------------------------
  memset( test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = get_string(root, "name", NULL, NULL, AB_MAX_LEN_TEST_NAME, 
      test_name); cBYE(status);
  //------------------------------------------
  status = get_int(root, "is_dev_specific", NULL, NULL, &is_dev_specific); cBYE(status);
  if ( ( is_dev_specific < 0 )  || ( is_dev_specific > 1 ) ) { go_BYE(-1); }
  //----------------------------------------
  status = get_test_idx(test_name, test_type, &test_idx); 
  if ( test_idx < 0 ) { 
    status = get_empty_spot(test_name, &test_idx); cBYE(status);
    if ( test_idx < 0 ) { go_BYE(-1); }
    status = malloc_test(test_idx, num_variants, is_dev_specific, state);
    cBYE(status);
  }
  else {
    if ( state == TEST_STATE_TERMINATED ) {
      if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
        /* nothing to do */
      }
      else if ( g_tests[test_idx].state == TEST_STATE_STARTED ) {
        // TODO TODO P1 
      }
      else {
        go_BYE(-1);
      }
    } 
    else if ( state == TEST_STATE_STARTED ) {
      if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
        go_BYE(-1); 
      }
      else if ( g_tests[test_idx].state == TEST_STATE_STARTED ) {
        /* nothing to do */
      }
      else {
        go_BYE(-1);
      }
    }
    else { // archived has been handled already 
      go_BYE(-1);
    }
  }

  json_decref(root);
BYE:
  return status;
}
#ifdef XXXX


  // cdata[0]=5;
  // printf("original value: %d\n", cdata[0]);
  go_BYE(-1); 
  //-------------------------------------
  // Now based on rslt, do things
#ifdef TODO
  int what_to_do      = rslt[0];
  int test_idx        = rslt[1]; 
  int num_variants    = rslt[2];
  int is_dev_specific = rslt[3];
  switch ( what_to_do ) { 
    case 1: // nil to started
      status = malloc_test(test_idx, num_variants, is_dev_specific,
          TEST_STATE_STARTED);
      break;
    case 2: // nil to terminated
      status = malloc_test(test_idx, num_variants, is_dev_specific,
          TEST_STATE_TERMINATED);
      break;
    case 3: // nil to archived => Nothing to do
      go_BYE(-1); 
      break;
    case 4: // stared to started => Nothing to do
      break;
    case 5: // started to terminated
      status = free_variant_per_bin(test_idx); cBYE(status);
      status = malloc_final_variant(test_idx); cBYE(status);
      break;
    case 6: // started to archived
      status = free_test(test_idx); 
      break;
    case 7: // terminated to started
      /* allowed for XYTests */ // TODO P1  RAMESH 
      break;
    case 8: // terminated to terminated
      go_BYE(-1);  // TODO P1 THINK ABOUT WHAT TO DO HERE 
      break;
    case 9: // terminated to archived
      status = free_test(test_idx); 
      break;
    default:
      go_BYE(-1);
      break;
  }
  cBYE(status);
  //-------------------------------------
  lua_getglobal(g_L, "add");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space : %s\n", lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  lua_pushlightuserdata(g_L, g_tests);
  entry_position[0] = test_idx;
  lua_pushlightuserdata(g_L, entry_position);
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    WHEREAMI; 
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    if (entry_position[0] != -1) {
      status = free_test(entry_position[0]); cBYE(status);
    }
    go_BYE(-1);
  }
  // printf("changed value: %d\n", cdata[0]);
#endif
BYE:
  return status;
}
#endif

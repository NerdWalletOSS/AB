#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "diagnostics.h"

int
l_diagnostics(
    const char *args
    )
{
  int status = 0;
  int bufsz = 7;
  char buf[bufsz+1]; 
  status = extract_name_value(args, "Source=", '&', buf, bufsz);
  cBYE(status);

  if ( ( *buf == '\0' ) || ( strcmp(buf, "Lua") == 0 ) ) {
    lua_getglobal(g_L, "diagnostics");
    if ( !lua_isfunction(g_L, -1)) {
      fprintf(stderr, "Function [diagnostics] does not exist \n");
      lua_pop(g_L, 1); go_BYE(-1);
    }
    status = lua_pcall(g_L, 0, 1, 0);
    if (status != 0) {
      fprintf(stderr, "calling function [diagnostics] failed: %s\n", lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
      // TODO P1 do we need to return here?
    }
    /* TODO Need to fill this out; Indrajeet to do  */
  }
  else if ( ( *buf != '\0' ) && ( strcmp(buf, "C") == 0 ) ) {
    status = diagnostics(); cBYE(status);
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}

int
diagnostics(
    )
{
  int status = 0;
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name_hash == 0 ) { 
      if ( g_tests[i].name[0] != '\0' ) { go_BYE(-1); }
      continue;
    }
    int test_type = g_tests[i].test_type;
    int state = g_tests[i].state;
    int has_filters = g_tests[i].has_filters;
    int is_dev_specific = g_tests[i].is_dev_specific;
    int num_variants = g_tests[i].num_variants;
    if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
         ( num_variants < AB_MAX_NUM_VARIANTS ) ) {
      go_BYE(-1);
    }
    float sum = 0;
    for ( int k = 0; k < num_variants; k++ ) {
      VARIANT_REC_TYPE vk = g_tests[i].variants[k];
      if ( test_type == AB_TEST_TYPE_XY ) { 
        if ( vk.url == NULL ) { go_BYE(-1); }
      }
      if ( vk.id <= 0 ) { go_BYE(-1); }
      sum += vk.percentage;
      if ( ( vk.percentage < 0 ) || ( vk.percentage > 100 ) ) { 
        go_BYE(-1); 
      }
      for ( int l = k+1; l < num_variants; l++ ) {
        VARIANT_REC_TYPE vl = g_tests[i].variants[l];
        if ( vl.id == vk.id ) { go_BYE(-1); }
        if ( strcmp(vl.name, vk.name ) == 0 ) { go_BYE(-1); }
      }
      if ( vk.url != NULL ) { 
        if ( ( strncmp(vk.url, "https://", 8) != 0 ) &&
            ( strncmp(vk.url, "http://", 7) != 0 ) ) {
          go_BYE(-1);
        }
        // TODO Check URL to make sure it is valid URL
      }
      if ( vk.custom_data != NULL ) {
        // TODO Make sure it is valid JSON
      }
    }
    if ( ( sum < 100-0.01 ) || ( sum > 100+0.01 ) ) { go_BYE(-1); }
    uint64_t external_id = g_tests[i].external_id;
    if ( test_type == AB_TEST_TYPE_AB ) { 
      if ( is_dev_specific ) { go_BYE(-1); }
    }
    else if ( test_type == AB_TEST_TYPE_XY ) { 
      if ( has_filters ) { go_BYE(-1); }
    }
    else {
      go_BYE(-1);
    }
    if ( ( state != TEST_STATE_STARTED ) &&
         ( state != TEST_STATE_TERMINATED ) ) { 
      go_BYE(-1);
    }
    for ( int j = i+1; j < AB_MAX_NUM_TESTS; j++ ) { 
      if ( g_tests[j].name_hash == 0 ) { 
        continue;
      }
      if ( g_tests[j].test_type == test_type ) { 
        if ( strcmp(g_tests[i].name, g_tests[j].name) == 0 ) {
          go_BYE(-1);
        }
      }
      if ( g_tests[j].test_type == AB_TEST_TYPE_XY ) { 
        if ( g_tests[j].external_id  == external_id ) {
          go_BYE(-1);
        }
      }
    }
  }
BYE:
  return status;
}

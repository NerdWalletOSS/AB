#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "l_diagnostics.h"

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
    if ( g_L == NULL ) { go_BYE(-1); }
    lua_getglobal(g_L, "diagnostics");
    if ( !lua_isfunction(g_L, -1)) {
      fprintf(stderr, "Function [diagnostics] does not exist \n");
      lua_pop(g_L, 1); go_BYE(-1);
    }
    status = lua_pcall(g_L, 0, 0, 0);
    if (status != 0) {
      fprintf(stderr, "calling function [diagnostics] failed: %s\n", lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
    }
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
  int counter[AB_MAX_NUM_VARIANTS];
  // we will collect all variant IDs in all_vids and test for uniqueness
  int all_vids[AB_MAX_NUM_VARIANTS*AB_MAX_NUM_TESTS]; int n_all_vids = 0;
  for ( int i = 0; i <  AB_MAX_NUM_VARIANTS; i++ ) { 
    counter[i] = 0;
  }
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name_hash == 0 ) {
      if ( g_tests[i].name[0]           != '\0' ) { go_BYE(-1); }
      if ( g_tests[i].test_type         != 0 ) { go_BYE(-1); }
      if ( g_tests[i].id                != 0 ) { go_BYE(-1); }
      if ( g_tests[i].external_id       != 0 ) { go_BYE(-1); }
      if ( g_tests[i].has_filters       != 0 ) { go_BYE(-1); }
      if ( g_tests[i].is_dev_specific   != 0 ) { go_BYE(-1); }
      if ( g_tests[i].state             != 0 ) { go_BYE(-1); }
      if ( g_tests[i].seed              != 0 ) { go_BYE(-1); }
      if ( g_tests[i].num_devices       != 0 ) { go_BYE(-1); }
      if ( g_tests[i].num_variants      != 0 ) { go_BYE(-1); }
      if ( g_tests[i].variants          != NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_id  != NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_idx != NULL ) { go_BYE(-1); }
      if ( g_tests[i].variant_per_bin  != NULL ) { go_BYE(-1); }
      continue;
    }
    int test_type = g_tests[i].test_type;
    int state = g_tests[i].state;
    int has_filters = g_tests[i].has_filters;
    int is_dev_specific = g_tests[i].is_dev_specific;
    uint32_t num_devices = g_tests[i].num_devices;
    if ( num_devices < 1 ) { go_BYE(-1); }
    int num_variants = g_tests[i].num_variants;
    if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
        ( num_variants > AB_MAX_NUM_VARIANTS ) ) {
      go_BYE(-1);
    }
    if ( g_tests[i].state == TEST_STATE_STARTED ) { 
      // not for terminated tests
      for ( uint32_t ii = 0; ii < num_devices; ii++ ) {
        for ( uint32_t jj = 0; jj < AB_NUM_BINS; jj++ ) {
          uint8_t x = g_tests[i].variant_per_bin[ii][jj];
          if ( x >= num_variants ) { go_BYE(-1); }
          counter[x]++;
        }
      }
    }
    float sum = 0;
    for ( int k = 0; k < num_variants; k++ ) {
      VARIANT_REC_TYPE vk = g_tests[i].variants[k];
      if ( ( test_type == AB_TEST_TYPE_XY ) && ( vk.url == NULL ) )  {
        go_BYE(-1);
      }
      if ( vk.url == NULL ) { 
        if ( vk.separator != '\0' ) { go_BYE(-1); }
      }
      else {
        if ( strlen(vk.url) > AB_MAX_LEN_URL ) { go_BYE(-1); }
        /* TODO P1 Put this check in 
           if ( ( vk.separator != '?' ) && ( vk.separator != '&' ) ) {
           go_BYE(-1);
           }
           */
      }
      if ( vk.id <= 0 ) { go_BYE(-1); }
      all_vids[n_all_vids++] = vk.id;
      sum += vk.percentage;
      if ( ( vk.percentage < 0 ) || ( vk.percentage > 100 ) ) { 
        go_BYE(-1); 
      }
      for ( int l = k+1; l < num_variants; l++ ) {
        VARIANT_REC_TYPE vl = g_tests[i].variants[l];
        if ( vl.id == vk.id ) { go_BYE(-1); }
        if ( ( vl.url != NULL ) && ( vk.url != NULL )  &&
            ( *vl.url != '\0' ) && ( *vk.url != '\0' ) ) { 
          if ( strcmp(vl.name, vk.name ) == 0 ) { go_BYE(-1); }
        }
      }
      if ( ( vk.url != NULL ) && ( *vk.url != '\0' ) ) {
        if ( ( strncmp(vk.url, "https://", 8) != 0 ) &&
            ( strncmp(vk.url, "http://", 7) != 0 ) ) {
          go_BYE(-1);
        }
        for ( char *cptr = vk.url; *cptr != '\0'; cptr++ ) { 
          if ( !is_valid_url_char(*cptr) ) { go_BYE(-1); }
        }
      }
      if ( vk.custom_data != NULL ) {
        if ( strlen(vk.custom_data) > AB_MAX_LEN_CUSTOM_DATA ) { go_BYE(-1); }
        // TODO P3 Make sure it is valid JSON
      }
    }
    // TODO P3 Check that counter[] is similar to percentage
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
    if ( g_tests[i].final_variant_id  != NULL ) { 
      for ( uint32_t d = 0; d < num_devices; d++ ) { 
        if ( g_tests[i].final_variant_id[d] <= 0 ) { 
          go_BYE(-1); 
        }
      }
      for ( uint32_t d = 0; d < num_devices; d++ ) { 
        if ( g_tests[i].final_variant_idx[d] >= (uint32_t)num_variants ) {
          go_BYE(-1);
        }
      }
    }

    if ( state == TEST_STATE_TERMINATED ) {

      if ( g_tests[i].variant_per_bin   != NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_id  == NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_idx == NULL ) { go_BYE(-1); }

    }
    else if ( state == TEST_STATE_STARTED ) {

      if ( g_tests[i].variant_per_bin == NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_id  != NULL ) { go_BYE(-1); }
      if ( g_tests[i].final_variant_idx != NULL ) { go_BYE(-1); }

    }
    else {
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
      if ( ( test_type == AB_TEST_TYPE_XY) && 
          ( g_tests[j].test_type == AB_TEST_TYPE_XY ) ) { 
        if ( g_tests[j].external_id  == external_id ) {
          go_BYE(-1);
        }
      }
    }
  }
  // all variant IDs must be unique 
  for ( int i = 0; i < n_all_vids; i++ ) { 
    for ( int j = i+1; j < n_all_vids; j++ ) { 
      if ( all_vids[i] == all_vids[j] ) {
        go_BYE(-1);
      }
    }
  }
BYE:
  return status;
}
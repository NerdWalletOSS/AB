#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "add_fake_test.h"

static int 
get_empty_spot(
    uint64_t name_hash,
    int *ptr_idx
    )
//</hdr>
{
  int status = 0; int idx = -1;
  int start = name_hash % AB_MAX_NUM_TESTS;
  for ( int i = start; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name_hash == 0 ) { idx = i; break; }
  }
  if ( idx < 0 ) { 
    for ( int i = 0; i < start; i++ ) { 
      if ( g_tests[i].name_hash == 0 ) { idx = i; break; }
    }
  }
  if ( idx < 0 ) { go_BYE(-1); }
  *ptr_idx = idx;
BYE:
  return status;
}

int
add_fake_test(
    const char *args
    )
{
  int status = 0;
  int idx = -1, vidx = -1;
  VARIANT_REC_TYPE *variants = NULL;
  uint8_t **vpb = NULL;
  int itemp; int nV = 0; // num variants 
  bool is_dev_specific;
  int test_type, state;
  int bufsz = 511; char *cptr = NULL;
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
  memset(buf, '\0', bufsz+1);
  status = extract_name_value(args, "State=", '&', buf, bufsz);
  cBYE(status);
  if ( buf[0] == '\0' ) { 
    state = TEST_STATE_STARTED;
  }
  else {
    if ( strcasecmp(buf, "started") == 0 ) {
      state = TEST_STATE_STARTED;
    }
    else if ( strcasecmp(buf, "terminated") == 0 ) {
      state = TEST_STATE_TERMINATED;
    }
    else {
      go_BYE(-1);
    }
  }
  //-----------------------------------------------
  memset(test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = extract_name_value(args, "TestName=", '&', test_name, 
      AB_MAX_LEN_TEST_NAME);
  cBYE(status);
  if ( test_name[0] == '\0' ) { go_BYE(-1); }
  //-----------------------------------------------
  memset(buf, '\0', bufsz+1);
  status = extract_name_value(args, "NumVariants=", '&', buf, bufsz);
  cBYE(status);
  status = stoI4(buf, &nV); cBYE(status);
  if ( ( nV < AB_MIN_NUM_VARIANTS ) || 
      ( nV > AB_MAX_NUM_VARIANTS ) ) { go_BYE(-1); }
  //-----------------------------------------------
  memset(buf, '\0', bufsz+1);
  status = extract_name_value(args, "IsDevSpecific=", '&', buf, bufsz);
  cBYE(status);
  if ( *buf != '\0' ) { 
    status = stoI4(buf, &itemp); cBYE(status);
    switch ( itemp ) { 
      case 0 : is_dev_specific = false; break; 
      case 1 : is_dev_specific = true; break; 
      default : fprintf(stderr, "%s\n", args); go_BYE(-1); break;
    }
  }
  else {
    is_dev_specific = false; 
  }
  //-----------------------------------------------
  //-- Test must not exist
  if ( ( is_dev_specific ) && ( test_type == AB_TEST_TYPE_AB ) ) {
    go_BYE(-1);
  }
  status = get_test_idx(test_name, test_type, &idx); 
  if ( ( status == 0 ) && ( idx >= 0 ) )  { go_BYE(-1); }
  // Find a spot 
  uint64_t name_hash = spooky_hash64(test_name, strlen(test_name), g_seed1);
  status = get_empty_spot(name_hash, &idx);  cBYE(status);
  if ( idx < 0 ) { go_BYE(-1); }
  //-----------------------------------------------
  strcpy(g_tests[idx].name, test_name);
  g_tests[idx].id = (name_hash & 0x0FFFFFFF) + test_type;
  g_tests[idx].name_hash = name_hash;
  g_tests[idx].external_id = name_hash;
  g_tests[idx].test_type = test_type;
  g_tests[idx].has_filters = false;
  g_tests[idx].state = state;
  g_tests[idx].seed = 9876543210;

  g_tests[idx].num_variants = nV;
  variants = malloc(nV * sizeof(VARIANT_REC_TYPE));
  return_if_malloc_failed(variants);
  //--- Set names
  strcpy(variants[0].name, "Control");
  for ( vidx = 1; vidx < nV; vidx++ ) { 
    sprintf(variants[vidx].name, "Variant%d", vidx);
  }
  //----- Set variant IDs
  int vid = name_hash & 0x0FFFFFFF;
  for ( vidx = 0; vidx  < nV; vidx++ ) { 
    variants[vidx].id = vid++;
  }
  for ( vidx = 0; vidx  < nV; vidx++ ) { 
    variants[vidx].percentage = 0;
  }
  // Set winner if terminated
  int winner_id = -1, winner_idx = -1;
  if ( state == TEST_STATE_TERMINATED ) { 
    winner_idx = name_hash % nV;
    winner_id = variants[winner_idx].id;
    if ( winner_id <= 0 ) { go_BYE(-1); }
    if ( winner_idx < 0 ) { go_BYE(-1); }
  }
  int sum = 0; vidx = 0;
  switch ( state ) { 
    case TEST_STATE_STARTED : 
     // Set percentages as equally as possible
      while ( sum < 100 ) { 
        variants[vidx++].percentage++; sum++;
        if ( vidx == nV ) { vidx = 0; }
      }
      break;
    case TEST_STATE_TERMINATED : 
      variants[winner_idx].percentage = 100;
      break;
    default : 
      go_BYE(-1);
      break;
  }
  for ( vidx = 0; vidx < nV; vidx++ ) { 
    memset(buf, '\0', bufsz+1);
    sprintf(buf, "http://localhost:8080/AB/test_webapp/index%d.html", vidx);
    cptr = strdup(buf); return_if_malloc_failed(cptr);
    variants[vidx].url = cptr;
    memset(buf, '\0', bufsz+1);
    sprintf(buf, "{ \"key%d\" : \"val%d\" } ", vidx, vidx);
    cptr = strdup(buf); return_if_malloc_failed(cptr);
    variants[vidx].custom_data = cptr;
  }
  g_tests[idx].variants = variants;
  // Determine numberof devices, nD
  int nD;
  if ( is_dev_specific ) { 
    nD = g_n_justin_cat_lkp;
  }
  else {
    nD = 1; // num devices 
  }
  if ( nD < 1 ) { go_BYE(-1); }

  // If device specific is not set, we use device_idx = 0
  switch ( state ) {
    case TEST_STATE_STARTED : 
      g_tests[idx].final_variant_id = NULL;
      g_tests[idx].final_variant_idx = NULL;
      vpb = malloc(nD * sizeof(uint8_t *));
      return_if_malloc_failed(vpb);
      for ( int i = 0; i < nD; i++ ) { 
        vpb[i] = malloc(AB_NUM_BINS * sizeof(uint8_t));
        return_if_malloc_failed(vpb[i]);
      }
      //---  Set bins 
      vidx = 0;
      for ( int d = 0; d < nD; d++ ) { 
        for ( int i = 0; i < AB_NUM_BINS; i++ ) { 
          if ( vidx == nV ) { vidx = 0; }
          vpb[d][i] = vidx++; 
        }
      }
      g_tests[idx].variant_per_bin = vpb;
      break;
    case TEST_STATE_TERMINATED : 
      g_tests[idx].final_variant_id = malloc(nD * sizeof(int));
      g_tests[idx].final_variant_idx = malloc(nD * sizeof(int));
      for ( int d = 0; d < nD; d++ ) { 
        g_tests[idx].final_variant_idx[d] = winner_idx;
        g_tests[idx].final_variant_id[d]  = winner_id;
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
  fprintf(stderr, "Created test %s at location %d \n", test_name, idx);

BYE:
  free_if_non_null(buf);
  return status;
}

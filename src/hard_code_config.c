#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "hard_code_config.h"
#include "get_test_idx.h"

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
void
hard_code_config(void)
{
  g_cfg.port = 8000;
  g_cfg.verbose = false;

  strcpy(g_cfg.logger.server,  "localhost");
  g_cfg.logger.port = 8004;
  strcpy(g_cfg.logger.url, "Log");
  strcpy(g_cfg.logger.health_url, "Ignore");

  strcpy(g_cfg.ss.server, "localhost");
  g_cfg.ss.port = 8024;
  strcpy(g_cfg.ss.url, "session");
  strcpy(g_cfg.ss.health_url, "api/v1/health_check");

  strcpy(g_cfg.statsd.server, "localhost");
  g_cfg.statsd.port = 8125;
  
  g_cfg.sz_log_q = 65536;
  g_cfg.num_post_retries = 1;

  strcpy(g_cfg.default_url, "http://localhost:8080/AB/test_webapp/index0.html");
  g_cfg.uuid_len = 8;
  g_cfg.xy_guid = 1;

  strcpy(g_cfg.ua_to_dev_map_file,  "/opt/ab/ua_to_dev.bin");
  strcpy(g_cfg.justin_cat_file,  "/opt/ab/justin_cat.csv");
  strcpy(g_cfg.os_file,  "/opt/ab/os.csv");
  strcpy(g_cfg.browser_file,  "/opt/ab/browser.csv");
  strcpy(g_cfg.device_type_file,  "/opt/ab/device_type.csv");

  strcpy(g_cfg.dt_file,  "/opt/ab/dt.bin");
  strcpy(g_cfg.rf_file,  "/opt/ab/rf.bin");
  strcpy(g_cfg.mdl_file,  "/opt/ab/mdl.bin");

  strcpy(g_cfg.mmdb_file,  "/opt/ab/GeoIP2-City_2017_12_08.mmdb");
}

int
add_fake_tests(
    const char *args
    )
{
  int status = 0;
  int idx = -1, vidx = -1;
  VARIANT_REC_TYPE *variants = NULL;
  uint8_t **vpb = NULL;
  int nV = 0; int test_type;
  int bufsz = 63;
  char test_name[AB_MAX_LEN_TEST_NAME+1]; char buf[bufsz+1];

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
  memset(buf, '\0', 8);
  status = extract_name_value(args, "NumVariants=", '&', buf, bufsz);
  cBYE(status);
  status = stoI4(buf, &nV); cBYE(status);
  if ( ( nV < AB_MIN_NUM_VARIANTS ) || 
       ( nV > AB_MAX_NUM_VARIANTS ) ) { go_BYE(-1); }
  //-----------------------------------------------
  //-- Test must not exist
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
  g_tests[idx].external_id = 1234567890;
  g_tests[idx].test_type = test_type;
  g_tests[idx].has_filters = false;
  g_tests[idx].is_dev_specific = false;
  g_tests[idx].state = TEST_STATE_STARTED;
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
  // Set percentages as equally as possible
  for ( vidx = 0; vidx  < nV; vidx++ ) { 
    variants[vidx].percentage = 0;
  }
  int sum = 0; vidx = 0;
  while ( sum <= 100 ) { 
    variants[vidx++].percentage++; sum++;
    if ( vidx == nV ) { vidx = 0; }
  }
  for ( vidx = 0; vidx < nV; vidx++ ) { 
    sprintf(buf, "http://localhost:8080/AB/test_webapp/index%d.html", vidx);
    variants[vidx].url = strdup(buf);
    sprintf(buf, "{ \"key%d\" : \"val%d\" } ", vidx, vidx);
    variants[vidx].custom_data = strdup(buf);
  }
  g_tests[idx].variants = variants;

  // If device specific is not set, we use device_idx = 0
  g_tests[idx].final_variant_id = NULL;
  g_tests[idx].final_variant_idx = NULL;
  vpb = malloc(1 * sizeof(uint8_t *));
  return_if_malloc_failed(vpb);
  vpb[0] = malloc(AB_NUM_BINS * sizeof(uint8_t));
  return_if_malloc_failed(vpb[0]);
  //---  Set bins 
  vidx = 0;
  for ( int i = 0; i < AB_NUM_BINS; i++ ) { 
    vpb[0][i] = vidx++; 
    if ( vidx == nV ) { vidx = 0; }
  }

  g_tests[idx].variant_per_bin = vpb;
BYE:
  return status;
}

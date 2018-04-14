#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "hard_code_config.h"
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

  strcpy(g_cfg.default_url, "http://localhost:8000/api/v1/health_check");
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
    void
    )
{
  int idx = 0;
  const char *name = "TestAB";
  VARIANT_REC_TYPE *variants = NULL;
  uint8_t **vpb = NULL;
  int nV = 3;

  strcpy(g_tests[idx].name, name);
  g_tests[idx].id = 1; 
  g_tests[idx].name_hash = spooky_hash64(name, strlen(name), g_seed1);
  g_tests[idx].external_id = 1234567890;
  g_tests[idx].test_type = AB_TEST_TYPE_AB;
  g_tests[idx].has_filters = false;
  g_tests[idx].is_dev_specific = false;
  g_tests[idx].state = TEST_STATE_STARTED;
  g_tests[idx].seed = 9876543210;

  g_tests[idx].num_variants = nV;
  variants = malloc(nV * sizeof(VARIANT_REC_TYPE));
  strcpy(variants[0].name, "Control");
  strcpy(variants[1].name, "VariantA");
  strcpy(variants[2].name, "VariantB");
  variants[0].id = 100;
  variants[1].id = 200;
  variants[2].id = 300;
  variants[0].percentage = 34;
  variants[1].percentage = 33;
  variants[2].percentage = 33;
  for ( int i = 0; i < nV; i++ ) { 
    variants[i].url = NULL;
    variants[i].custom_data = NULL;
  }
  g_tests[idx].variants = variants;

  // If device specific is not set, we use device_idx = 0
  g_tests[idx].final_variant_id = NULL;
  g_tests[idx].final_variant_idx = NULL;
  vpb = malloc(1 * sizeof(uint8_t *));
  vpb[0] = malloc(AB_NUM_BINS * sizeof(uint8_t));
  int vidx = 0;
  for ( int i = 0; i < AB_NUM_BINS; i++ ) { 
    vpb[0][i] = vidx++; 
    if ( vidx == nV ) { vidx = 0; }
  }

  g_tests[idx].variant_per_bin = vpb;
}

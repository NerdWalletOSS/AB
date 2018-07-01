#include "ab_incs.h"
#include "macros.h"
#include "make_guid.h"
#include "auxil.h"
#include "execute.h"
#include "setup_curl.h"
#include "read_test_info.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ofp = NULL; // for timing measurements
  CURL *ch = NULL;
  long http_code; int itemp;
  int *H = NULL; int nH = 1000; // histogram  for response times 
  TEST_INFO_REC_TYPE *T = NULL;
  int num_tests = 0;

  // Set globals 

  // process input parameters
  if ( argc != 6 ) { go_BYE(-1); }

  char *server         = argv[1];

  status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp < 80 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  int32_t port = (int32_t)itemp; 

  char *test_file_name = argv[3];

  status = stoI4(argv[4], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  int32_t num_iters = (int32_t)itemp; 

  status = stoI4(argv[5], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  int32_t num_users = (int32_t)itemp; 

  //----------------------------------------
  int start_uuid = 12345678;
  H = malloc(nH * sizeof(int)); return_if_malloc_failed(H);

  status = read_test_info(test_file_name, &T, &num_tests); cBYE(status);
  if ( ( T == NULL ) || ( num_tests == 0 ) ) { go_BYE(-1); }
  status = setup_curl(&ch); cBYE(status);
  // Set base URL

  double avg_time = 0;
  double min_time = INT_MAX;
  double max_time = INT_MIN;

  int num_bad = 0; 
  uint64_t t1, t2, t;
  int hit_ctr = 0; // number of hits with valid responses
  int ndots = 0;
  int num_over = 0;
  int exp_num_hits = num_iters * num_users * num_tests;
  // Get variants for these tests for num_users users
  for ( int iter = 0; iter < num_iters; iter++ ) {
    // T is used to measure individual times 
    for ( int uid = 0; uid < num_users; uid++ ) {
      for ( int test_id = 0; test_id < num_tests; test_id++ ) { 
        char url[AB_MAX_LEN_URL+1];
        char tracer[AB_MAX_LEN_TRACER+1];
        for ( int i = 0; i < AB_MAX_LEN_URL+1; i++ ) { url[i] = '\0'; }
        for ( int i = 0; i < AB_MAX_LEN_TRACER+1; i++ ) { tracer[i] = '\0';}
        status = make_guid(NULL, tracer, AB_MAX_LEN_TRACER); 
        cBYE(status);
        sprintf(url, "%s:%d/GetVariant?TestName=%s&TestType=%s&UUID=%d&Tracer=%s", 
            server, port,
            T[test_id].test_name, 
            T[test_id].test_type, 
            start_uuid+uid, tracer);
        t1 = get_time_usec();
        // fprintf(stderr, "%s\n", url);
        execute(ch, url, &http_code);
        if ( num_bad > 10000 ) { goto DONE; }
        if ( http_code != 200 ) { continue; num_bad++; }
        t2 = get_time_usec();
        if ( t2 < t1 ) {  continue; }
        t = t2 - t1;
        if ( t > max_time ) { max_time = t; }
        if ( t < min_time ) { min_time = t; }
        avg_time += t;
        hit_ctr++;
        t = t % 1000; // convert to msec
        if ( (int)t >= nH ) { t = nH; num_over++; }
        H[t]++;
        if ( ( hit_ctr % 500 ) == 0 ) {
          sprintf(url, "%s:%d/Diagnostics?Source=C", server, port);
          status = execute(ch, url, &http_code); cBYE(status);
          if ( http_code != 200 ) { go_BYE(-1); }
          ndots++; 
          fprintf(stderr, ".");
          if ( ndots >= 64 ) { 
            fprintf(stderr, "%d/%d\n", hit_ctr, exp_num_hits);
            ndots = 0;
          }
        }
      }
    }
  }
DONE:
  fprintf(stderr, "num_over = %d \n", num_over);
  avg_time /= hit_ctr;
  fprintf(stderr, "num_bad   \t = %d\n", num_bad);
  fprintf(stderr, "num_tests \t = %d\n", num_tests);
  fprintf(stderr, "num_trials\t = %d\n", hit_ctr);
  fprintf(stderr, "num_users \t = %d\n", num_users);
  fprintf(stderr, "avg_time  \t = %f\n",  avg_time);
  fprintf(stderr, "min_time  \t = %f\n",  min_time);
  fprintf(stderr, "max_time  \t = %f\n",  max_time);
  curl_easy_cleanup(ch); ch = NULL;

  ofp = fopen("_timings.csv", "w");
  return_if_fopen_failed(ofp, "_timings.csv", "w");
  for (  int i = 0; i < nH; i++ ) { 
    fprintf(ofp, "%d,%d\n", i, H[i]);
  }
  fclose_if_non_null(ofp);
BYE:
  fclose_if_non_null(ofp);
  if ( ch != NULL ) { curl_easy_cleanup(ch); }
  free_if_non_null(T);
  return status ;
}

#include "incs.h"
#include "ab_constants.h"
#include "add_tests.h"
#include "auxil.h"
#include "execute.h"
#include "setup_curl.h"

int g_chunk_size;
char *g_chunk;

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
#define MAX_LEN_SERVER_NAME 127
#define MAX_LEN_URL 127
  char server[MAX_LEN_SERVER_NAME+1];
  char url[MAX_LEN_URL+1];
  char base_url[MAX_LEN_SERVER_NAME+1+64]; 
  char **test_urls = NULL;
  int *H = NULL; int nH = 1000; // histogram  for response times 

  // Set globals 
  g_chunk_size = 16384; g_chunk = NULL;
  g_chunk = malloc(g_chunk_size * sizeof(char));
  return_if_malloc_failed(g_chunk);

  // process input parameters
  if ( argc != 3 ) { go_BYE(-1); }

  memset(server, '\0', MAX_LEN_SERVER_NAME+1);
  strcpy(server, argv[1]); 

  status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp < 80 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  uint16_t port = (uint16_t)itemp; 
  //----------------------------------------
  int num_tests = 16;
  int niter = 8;
  int nU = 1024; 
  int start_uuid = 12345678;
  H = malloc(nH * sizeof(int)); return_if_malloc_failed(H);

  status = setup_curl(&ch); cBYE(status);
  // Set base URL
  memset(base_url, '\0', (MAX_LEN_SERVER_NAME+1+64));
  sprintf(base_url, "%s:%d", server, port);
  // Restart AB RTS
  fprintf(stderr, "Restarting the ab server\n");
  sprintf(url, "%s:%d/Restart", server, port);
  status = execute(ch, url, &http_code); cBYE(status);
  if ( http_code != 200 ) { go_BYE(-1); }
  //-- Add a bunch of tests 
  status = add_tests(ch, server, port, num_tests, &test_urls); cBYE(status);

  double avg_time = 0;
  double min_time = INT_MAX;
  double max_time = INT_MIN;

  curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, 10);
  // Get variants for these tests for nU users
  int num_bad = 0; 
  uint64_t t1, t2, t;
  // T is used to measure individual times 
  int hit_ctr = 0; // number of hits with valid responses
  int ndots = 0;
  int num_over = 0;
  int exp_num_hits = niter * nU * num_tests; 
  for ( int iter = 0; iter < niter; iter++ ) {
    for ( int uid = 0; uid < nU; uid++ ) {
      for ( int test_id = 0; test_id < num_tests; test_id++ ) { 
        sprintf(url, "%s&UUID=%d", test_urls[test_id], start_uuid+uid);
        t1 = get_time_usec();
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
  status = del_tests(ch, server, port, num_tests); cBYE(status);
DONE:
  fprintf(stderr, "num_over = %d \n", num_over);
  avg_time /= hit_ctr;
  fprintf(stderr, "num_bad   \t = %d\n", num_bad);
  fprintf(stderr, "num_tests \t = %d\n", num_tests);
  fprintf(stderr, "num_trials\t = %d\n", hit_ctr);
  fprintf(stderr, "num_users \t = %d\n", nU);
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
  if ( test_urls != NULL ) { 
    for ( int test_id = 0; test_id < num_tests; test_id++ ) {
      free_if_non_null(test_urls[test_id]);
    }
  }
  free_if_non_null(test_urls);
  if ( ch != NULL ) { curl_easy_cleanup(ch); }
  free_if_non_null(g_chunk);
  return status ;
}

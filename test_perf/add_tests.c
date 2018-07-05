#include "incs.h"
#include "ab_constants.h"
#include "execute.h"
#include "add_tests.h"

static inline uint64_t 
RDTSC(void)
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

int
add_tests(
    CURL *ch,
    char *server,
    int port,
    int num_tests,
    char *op_file_name
    )
{
  int status = 0;
  long http_code;
  char **test_urls = NULL;
  FILE *ofp = NULL;

  ofp = fopen(op_file_name, "w");
  return_if_fopen_failed(ofp,  op_file_name, "w");
  int num_variants = AB_MIN_NUM_VARIANTS;
  char url[AB_MAX_LEN_URL+1];

  curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, 10000); 
  // allocate test URLs
  test_urls = malloc(num_tests * sizeof(char *));
  return_if_malloc_failed(test_urls);
  for ( int test_id = 0; test_id < num_tests; test_id++ ) {
    test_urls[test_id] = NULL;
  }
  for ( int test_id = 0; test_id < num_tests; test_id++ ) {
    test_urls[test_id] = malloc(AB_MAX_LEN_URL+1 * sizeof(char));
    return_if_malloc_failed(test_urls[test_id]);
    memset(test_urls[test_id], '\0',  AB_MAX_LEN_URL+1);
  }
  //----------------------------
  fprintf(stderr, "Creating %d tests T1, T2, .. \n", num_tests);
  for ( int test_id = 0; test_id < num_tests; test_id++ ) {
    int is_dev_specific;
    char test_type[16]; memset(test_type, '\0', 16);
    char state[16];     memset(state,     '\0', 16);
    //---------------------------------
    if ( ( RDTSC() & 1 ) == 1 ) { 
      strcpy(test_type, "ABTest"); 
    }
    else {
      strcpy(test_type, "XYTest"); 
    }
    //---------------------------------
    if ( ( RDTSC() & 1 ) == 1 ) { 
      strcpy(state, "terminated");
    }
    else {
      strcpy(state, "started");
    }
    //------------------------------
    if ( ( RDTSC() & 1 ) == 1 ) { 
      is_dev_specific = 1;
    }
    else {
      is_dev_specific = 0;
    }
    if ( strcmp(test_type, "ABTest") == 0 ) { 
      is_dev_specific = 0;
    }
    num_variants = (RDTSC() % (AB_MAX_NUM_VARIANTS - AB_MIN_NUM_VARIANTS) )
      +AB_MIN_NUM_VARIANTS;
    //---------------------------------------------
    fprintf(ofp,"T%d,%s,%s,%d,%d\n",
        test_id, test_type, state, is_dev_specific, num_variants);
    //---------------------------------------------
    memset(url, '\0', AB_MAX_LEN_URL+1);
    sprintf(url, "%s:%d/AddFakeTest?TestName=T%d&TestType=%s&State=%s&IsDevSpecific=%d&NumVariants=%d", 
        server, port, test_id, test_type, state, is_dev_specific, num_variants);
    // fprintf(stderr, "%s\n", url);
    status = execute(ch, url, &http_code); cBYE(status);
    if ( http_code != 200 ) { go_BYE(-1); }
  }
BYE:
  fclose_if_non_null(ofp);
  return status;
}

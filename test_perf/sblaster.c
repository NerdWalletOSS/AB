#include "ab_incs.h"
#include <float.h>
#include "macros.h"
#include "make_guid.h"
#include "auxil.h"
#include "execute.h"
#include "setup_curl.h"
#include "read_test_info.h"
#include "hammer.h"

CURL **g_ch; // [g_num_threads] 
int32_t g_num_threads;
char *g_server;
int32_t g_port;
int32_t g_num_users;
int32_t g_num_iters;
TEST_INFO_REC_TYPE *g_T; // [g_num_tests] 
int32_t g_num_tests;

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  g_ch = NULL;
  g_T = NULL;

  // Set globals 
  // process input parameters
  if ( argc != 7 ) { go_BYE(-1); }

  g_server         = argv[1];

  int itemp; status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp < 80 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  g_port = (int32_t)itemp; 

  char *test_file_name = argv[3];

  status = stoI4(argv[4], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  g_num_iters = (int32_t)itemp; 

  status = stoI4(argv[5], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  g_num_users = (int32_t)itemp; 

  status = stoI4(argv[6], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  g_num_threads = (int32_t)itemp; 

  //----------------------------------------
  status = read_test_info(test_file_name, &g_T, &g_num_tests); cBYE(status);
  if ( ( g_T == NULL ) || ( g_num_tests == 0 ) ) { go_BYE(-1); }
  // Note that we allocate an array of 1 to keep this similar to 
  // parallel blaster code
  g_ch = malloc(g_num_threads * sizeof(CURL *));
  return_if_malloc_failed(g_ch);
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    status = setup_curl(&(g_ch[tid])); cBYE(status);
  }
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    hammer((void *)&tid); 
  }
/*
  fprintf(stderr, "num_bad  \t = %d\n", num_bad);
  fprintf(stderr, "num_good \t = %d\n", num_good);
  fprintf(stderr, "avg_time \t = %f\n", total_time/num_good);
  fprintf(stderr, "min_time \t = %f\n", min_time);
  fprintf(stderr, "max_time \t = %f\n", max_time);
*/
BYE:
  if ( g_ch != NULL ) { 
    if ( g_ch[0] != NULL ) { curl_easy_cleanup(g_ch[0]); }
  }
  free_if_non_null(g_ch);
  free_if_non_null(g_T);
  return status ;
}

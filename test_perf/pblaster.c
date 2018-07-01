/*
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <curl/curl.h>
*/
#include <pthread.h>
#include "ab_incs.h"
#include "auxil.h"
#include "execute.h"
#include "add_tests.h"
#include "setup_curl.h"
#include "ab_constants.h"
#include "read_test_info.h"
#include "hammer.h"

#undef SEQUENTIAL

CURL **g_ch; // [g_num_threads]
int g_num_threads; 
char *g_server;
int32_t g_port;
int g_num_users;
int g_num_iters; 
TEST_INFO_REC_TYPE *g_T; // [num_tests]
int32_t g_num_tests;

typedef struct _thread_info_type {
  int status;
  int tid;
  uint64_t time_taken;
  int num_good;
  int num_bad;
} THREAD_INFO_TYPE;


int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
#define MAX_LEN_SERVER_NAME 511
  pthread_t *threads = NULL;
  THREAD_INFO_TYPE *tinfo = NULL;
  g_ch = NULL;
  g_T = NULL;

  g_num_threads = 8; // TODO UNDO HARD CODING 
  if ( argc != 6 ) { go_BYE(-1); }

  g_server =  argv[1];

  int itemp; status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp <= 80 ) || (itemp >= 65536 ) ) { go_BYE(-1); }
  g_port = (uint16_t)itemp; 

  char *test_file_name = argv[3];

  status = stoI4(argv[4], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  g_num_iters = (int32_t)itemp; 

  status = stoI4(argv[5], &itemp); cBYE(status);
  if ( itemp <= 0 ) { go_BYE(-1); }
  g_num_users = (int32_t)itemp; 

  status = read_test_info(test_file_name, &g_T, &g_num_tests); cBYE(status);
  if ( ( g_T == NULL ) || ( g_num_tests == 0 ) ) { go_BYE(-1); }

  g_ch = malloc(g_num_threads * sizeof(CURL *));
  return_if_malloc_failed(g_ch);
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    status = setup_curl(&(g_ch[tid])); cBYE(status);
  }

  threads = malloc(g_num_threads * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  memset(threads, '\0', g_num_threads * sizeof(pthread_t));

  tinfo = malloc(g_num_threads * sizeof(THREAD_INFO_TYPE));
  return_if_malloc_failed(tinfo);
  memset(tinfo, '\0', g_num_threads * sizeof(THREAD_INFO_TYPE));

  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    tinfo[tid].tid        = tid;
    tinfo[tid].status     = 0;
    tinfo[tid].time_taken = 0;
    tinfo[tid].num_good   = 0;
    tinfo[tid].num_bad    = 0;
  }


  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    pthread_create(&(threads[tid]), NULL, hammer, (void *)(&tid));
  }
  fprintf(stderr, "forked all threads\n");
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    pthread_join(threads[tid], NULL);
  }
  fprintf(stderr, "joined all threads\n");
BYE:
  if ( g_ch != NULL ) { 
    for ( int tid = 0; tid < g_num_threads; tid++ ) { 
      if ( g_ch[tid] != NULL ) { 
        curl_easy_cleanup(g_ch[tid]); 
        g_ch[tid] = NULL;
      }
    }
    curl_global_cleanup();
  }
  free_if_non_null(threads);
  free_if_non_null(tinfo);
  free_if_non_null(g_T);
  return status ;
}

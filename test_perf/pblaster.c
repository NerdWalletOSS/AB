#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "macros.h"
#include "auxil.h"
#include "execute.h"
#include "add_tests.h"
#include "setup_curl.h"
#include "ab_constants.h"
#include "restart.h"

#undef SEQUENTIAL

char *g_chunk;
int g_chunk_size;
int g_num_tests;
int g_num_iter; 
int g_num_threads; 
char **g_test_urls;
CURL **g_pch;

#define WRITEBACK_BUF_SIZE 1023

typedef struct _thread_info_type {
  int status;
  int tid;
  uint64_t time_taken;
  int num_good;
  int num_bad;
  void *writeback_buf;
} THREAD_INFO_TYPE;


static size_t 
WriteMemoryCallback(
    void *contents, 
    size_t size, 
    size_t nmemb, 
    void *userp
    )
{
  size_t realsize = size * nmemb;
  return realsize;
}

static int
p_execute(
    CURL *ch,
    const char *url,
    long *ptr_http_code
    )
{
  int status = 0; 
  CURLcode curl_res;
  
  curl_easy_setopt(ch, CURLOPT_URL, url);
  curl_res = curl_easy_perform(ch);
  if ( curl_res < 0 ) { go_BYE(-1); }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, ptr_http_code);
BYE:
  return status;
}

extern void * hammer( void *X);

void *
    hammer(
        void *X
        )
{
  THREAD_INFO_TYPE *ptr_tinfo;
  ptr_tinfo = (THREAD_INFO_TYPE *)X;

  char url[AB_MAX_LEN_URL+1];
  long http_code;

  int niter        = g_num_iter;
  int num_tests    = g_num_tests; 

  int tid          = ptr_tinfo->tid;
  if ( ( tid < 0 ) || ( tid >= g_num_threads ) )  {
    WHEREAMI; return NULL;
  }
  char **test_urls = g_test_urls;
  CURL *ch         = g_pch[tid];

  int num_good = 0; int num_bad = 0;
  srandom(timestamp());

  for ( int iter = 0; iter < niter; iter++ ) {
    int status = 0;
    uint64_t uuid = abs(random());
    uint64_t t_start, t_stop;
    memset(url, '\0', AB_MAX_LEN_URL+1);
    int test_id = abs(random()) % num_tests;
    const char *test_url = test_urls[test_id];

    sprintf(url, "%s&UUID=%" PRIu64 , test_url, uuid);
    t_start = RDTSC();
    status = p_execute(ch, url, &http_code); 
    t_stop = RDTSC();
    if ( ( status < 0 ) || ( http_code == 200 ) ) { 
      num_good++;
      ptr_tinfo->time_taken += (t_stop - t_start);
    }
    else {
      num_bad++;
    }
  }
  fprintf(stderr, "%3d Completed\n", tid);
  ptr_tinfo->num_good = num_good;
  ptr_tinfo->num_bad = num_bad;
  return NULL;
}

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
#define MAX_LEN_SERVER_NAME 511
  char server[MAX_LEN_SERVER_NAME+1];
  pthread_t *threads = NULL;
  THREAD_INFO_TYPE *tinfo = NULL;
  CURL *ch = NULL;
  int g_num_ports = 1; 
  int *g_ports = NULL;

  g_chunk = NULL; g_chunk_size = 16384-1;

  g_chunk = malloc(g_chunk_size+1);
  return_if_malloc_failed(g_chunk);
  memset(g_chunk, '\0', g_chunk_size+1);

  status = setup_curl(&ch); cBYE(status);

  if ( argc != 4 ) { go_BYE(-1); }

  memset(server, '\0',MAX_LEN_SERVER_NAME+1);
  strcpy(server, argv[1]); 

  int itemp;
  // Set logger port 
  status = stoI4(argv[3], &itemp); cBYE(status);
  if ( itemp < 0 ) { go_BYE(-1); }
  uint16_t log_port = (uint16_t)itemp; 

  //--- START: Set ab ports
  for ( char *cptr = argv[2]; *cptr != '\0'; cptr++ ) {
    if ( *cptr == ':' ) { g_num_ports++; }
  }
  g_ports = malloc(g_num_ports * sizeof(int));
  return_if_malloc_failed(g_ports);
  for ( int i = 0; i < g_num_ports; i++ ) { 
    char *x;
    if ( i == 0 ) { 
      x = strtok(argv[2], ":");
    }
    else {
      x = strtok(NULL, ":");
    }
    status = stoI4(x, &itemp); cBYE(status);
    if ( ( itemp <= 1024 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
    g_ports[i] = itemp;
    if ( itemp == log_port ) { go_BYE(-1); }
  }
  //-------------------------------

  // initialize globals
  g_num_iter   = 1024;
  g_num_tests  = 16; // TODO UNDO HARD CODING 
  g_test_urls  = NULL;
  g_pch        = NULL;
  g_num_threads = 128; // TODO UNDO HARD CODING 

  for ( int i = 0; i < g_num_ports; i++ ) { 
   status = restart(ch, server, g_ports[i], "Restart"); cBYE(status);
    //-- Add a bunch of tests 
    status = add_tests(ch, server, g_ports[i], g_num_tests, &g_test_urls); 
    cBYE(status);
  }
  status = restart(ch, server, log_port, "Restart"); cBYE(status);

  threads = malloc(g_num_threads * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  memset(threads, '\0', g_num_threads * sizeof(pthread_t));

  tinfo = malloc(g_num_threads * sizeof(THREAD_INFO_TYPE));
  return_if_malloc_failed(tinfo);
  memset(tinfo, '\0', g_num_threads * sizeof(THREAD_INFO_TYPE));

  g_pch = malloc(g_num_threads * sizeof(CURL *));
  return_if_malloc_failed(g_pch);
  memset(g_pch, '\0',  (g_num_threads * sizeof(CURL *)));

  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    tinfo[tid].tid        = tid;
    tinfo[tid].status     = 0;
    tinfo[tid].time_taken = 0;
    tinfo[tid].num_good   = 0;
    tinfo[tid].num_bad    = 0;
    tinfo[tid].writeback_buf = malloc(WRITEBACK_BUF_SIZE+1);
    return_if_malloc_failed(tinfo[tid].writeback_buf);
  }


  for ( int tid = 0; tid < g_num_threads; tid++ ) {
    g_pch[tid] = curl_easy_init();
    // insecure is okay
    curl_easy_setopt(g_pch[tid], CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(g_pch[tid], CURLOPT_SSL_VERIFYPEER, 0);
    /* send all data to this function  */ 
    curl_easy_setopt(g_pch[tid], CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(g_pch[tid], CURLOPT_WRITEDATA, 
        (void *)(tinfo[tid].writeback_buf));
  }

  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
#ifdef SEQUENTIAL
    hammer((void *)&(tinfo[tid]));
#else
    // fprintf(stderr, "Forking thread %d \n", tid);
    pthread_create(&(threads[tid]), NULL, hammer, (void *)&(tinfo[tid]));
#endif
  }
#ifndef SEQUENTIAL 
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    pthread_join(threads[tid], NULL);
  }
#endif
  uint64_t total_time = 0; 
  uint64_t total_hits = 0;
  uint64_t total_errs = 0;
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    total_time += tinfo[tid].time_taken;
    total_hits += tinfo[tid].num_good;
    total_errs += tinfo[tid].num_bad;
  }
  fprintf(stderr, "Total time = %" PRIu64 "\n", total_time);
  fprintf(stderr, "Total hits = %" PRIu64 "\n", total_hits);
  fprintf(stderr, "Total errs = %" PRIu64 "\n", total_errs);
BYE:
  if ( g_pch != NULL ) { 
    for ( int tid = 0; tid < g_num_threads; tid++ ) { 
      if ( g_pch[tid] != NULL ) { 
        // printf("cleaning %d \n", tid);
        curl_easy_cleanup(g_pch[tid]); 
        g_pch[tid] = NULL;
      }
    }
    curl_global_cleanup();
  }
  // printf("Completed curl cleanup\n");
  free_if_non_null(threads);
  free_if_non_null(tinfo);
  free_if_non_null(g_chunk);
  if ( g_test_urls != NULL ) { 
    for ( int test_id = 0; test_id < g_num_tests; test_id++ ) {
      free_if_non_null(g_test_urls[test_id]);
    }
  }
  free_if_non_null(g_test_urls);
  free_if_non_null(g_ports);
  if ( ch != NULL ) { curl_easy_cleanup(ch); ch = NULL; }
  return status ;
}

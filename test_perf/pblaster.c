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

char *g_chunk;
int g_chunk_size;

#define WRITEBACK_BUF_SIZE 1023

typedef struct _thread_info_type {
  int status;
  int tid;
  int niter;
  int num_tests;
  uint64_t time_taken;
  int num_good;
  int num_bad;
  CURL *ch;
  void *writeback_buf;
  char **test_urls;
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
  memcpy(userp, contents, realsize);
  // fprintf(stderr, "Copied %d bytes. First 32 bytes = ", (int)realsize);
  /*
  for ( int i = 0; i < 64; i++ ) { 
    fprintf(stderr, "%c", ((char *)userp)[i]);
  }
  fprintf(stderr, "\n");
  */
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

  int tid          = ptr_tinfo->tid;
  int niter        = ptr_tinfo->niter;
  int num_tests    = ptr_tinfo->num_tests;
  char **test_urls = ptr_tinfo->test_urls;
  CURL*ch          = ptr_tinfo->ch;

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
  CURL **pch = NULL;
#define MAX_LEN_SERVER_NAME 511
  char server[MAX_LEN_SERVER_NAME+1];
  pthread_t *threads = NULL;
  THREAD_INFO_TYPE *tinfo = NULL;
  char **test_urls = NULL;
  CURL *ch = NULL;


  g_chunk = NULL; g_chunk_size = 16384-1;

  g_chunk = malloc(g_chunk_size+1);
  return_if_malloc_failed(g_chunk);
  memset(g_chunk, '\0', g_chunk_size+1);

  status = setup_curl(&ch); cBYE(status);

  if ( argc != 4 ) { go_BYE(-1); }

  memset(server, '\0',MAX_LEN_SERVER_NAME+1);
  strcpy(server, argv[1]); 

  int itemp; 
  int num_ports = 10; // TODO FIX 
  status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp <= 1024 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  uint16_t ab_port = (uint16_t)itemp; 

  status = stoI4(argv[3], &itemp); cBYE(status);
  if ( itemp < 0 ) { go_BYE(-1); }
  uint16_t log_port = (uint16_t)itemp; 

  if ( log_port == ab_port ) { go_BYE(-1); }

  int niter       = 1024;
  int num_tests   = 16; // TODO UNDO HARD CODING 
  int num_threads = 128;
  //-- Add a bunch of tests 
  status = add_tests(ch, server, ab_port, num_tests, &test_urls); 
  cBYE(status);
  threads = malloc(num_threads * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  tinfo = malloc(num_threads * sizeof(THREAD_INFO_TYPE));
  return_if_malloc_failed(tinfo);

  for ( int tid = 0; tid < num_threads; tid++ ) { 
    tinfo[tid].writeback_buf = malloc(WRITEBACK_BUF_SIZE+1);
  }
  pch = malloc(num_threads * sizeof(CURL *));
  return_if_malloc_failed(pch);
  for ( int tid = 0; tid < num_threads; tid++ ) {  pch[tid] = NULL; }
  for ( int tid = 0; tid < num_threads; tid++ ) {
    pch[tid] = curl_easy_init();
    // insecure is okay
    curl_easy_setopt(pch[tid], CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(pch[tid], CURLOPT_SSL_VERIFYPEER, 0);
    /* send all data to this function  */ 
    curl_easy_setopt(pch[tid], CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(pch[tid], CURLOPT_WRITEDATA, 
        (void *)&(tinfo[tid].writeback_buf));
  }

#undef SEQUENTIAL
  for ( int tid = 0; tid < num_threads; tid++ ) { 
    tinfo[tid].tid        = tid;
    tinfo[tid].status     = 0;
    tinfo[tid].num_tests  = num_tests;
    tinfo[tid].niter      = niter;
    tinfo[tid].time_taken = 0;
    tinfo[tid].num_good   = 0;
    tinfo[tid].num_bad    = 0;
    tinfo[tid].test_urls  = test_urls;
    tinfo[tid].ch         = pch[tid];
#ifdef SEQUENTIAL
    hammer((void *)&(tinfo[tid]));
#else
    fprintf(stderr, "Forking thread %d \n", tid);
    pthread_create(&(threads[tid]), NULL, hammer, (void *)&(tinfo[tid]));
#endif
  }
  for ( int tid = 0; tid < num_threads; tid++ ) { 
    pthread_join(threads[tid], NULL);
  }
  double total_time = 0; double total_hits = 0;
  for ( int tid = 0; tid < num_threads; tid++ ) { 
    total_time += tinfo[tid].time_taken;
    total_hits += tinfo[tid].num_good;
  }
  fprintf(stderr, "Time/hit = %lf \n", (total_time/1000.0)/total_hits);
BYE:
  if ( pch != NULL ) { 
    for ( int tid = 0; tid < num_threads; tid++ ) { 
      if ( pch[tid] != NULL ) { 
        printf("cleaning %d \n", tid);
        curl_easy_cleanup(pch[tid]); 
        pch[tid] = NULL;
      }
    }
    curl_global_cleanup();
  }
  printf("Completed curl cleanup\n");
  free_if_non_null(threads);
  free_if_non_null(tinfo);
  free_if_non_null(g_chunk);
  if ( test_urls != NULL ) { 
    for ( int test_id = 0; test_id < num_tests; test_id++ ) {
      free_if_non_null(test_urls[test_id]);
    }
  }
  free_if_non_null(test_urls);
  return status ;
}

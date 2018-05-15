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

#define CHUNK_SIZE 16384
char g_chunk[CHUNK_SIZE];
char g_base_url[1024];

typedef struct _thread_info_type {
  int status;
  int tid;
  int num_tests;
  int niter;
  int nU;
  int start_uuid;
  uint64_t time_taken;
  int num_hits;
  CURL *ch;
} THREAD_INFO_TYPE;

extern int execute(
    CURL *ch,
    const char *url
    ) ;

int
execute(
    CURL *ch,
    const char *url
    )
{
  int status = 0; 
  CURLcode curl_res;
  long http_code;
  curl_easy_setopt(ch, CURLOPT_URL, url);
  curl_res = curl_easy_perform(ch);
  if ( curl_res < 0 ) { go_BYE(-1); }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
  if ( http_code != 200 ) { go_BYE(-1); }
  // fprintf(stderr, "return = %s \n", g_chunk);
BYE:
  memset(g_chunk, '\0', CHUNK_SIZE);
  return status;
}

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

void *
    hammer(
        void *X
        )
{
  int status = 0;
  THREAD_INFO_TYPE *ptr_tinfo;
  ptr_tinfo = (THREAD_INFO_TYPE *)X;
  char url[1024];

  int tid        = ptr_tinfo->tid;
  int niter      = ptr_tinfo->niter;
  int nU         = ptr_tinfo->nU;
  int num_tests  = ptr_tinfo->num_tests;
  int start_uuid = ptr_tinfo->start_uuid;
  CURL*ch        = ptr_tinfo->ch;

  printf("num_ports = %d \n", num_ports);
  // fprintf(stderr, "niter = %d \n", niter);
  // fprintf(stderr, "nU    = %d \n", nU);
  int num_hits = 0;
  for ( int iter = 0; iter < niter; iter++ ) {
    for ( int uid = 0; uid < nU; uid++ ) { 
      memset(url, '\0', 1024);
      int test_id = abs(random()) % num_tests;

      sprintf(url, "%s/GetVariant?TestName=T%d&TestType=ABTest&UUID=%d", 
          g_base_url, test_id+1, start_uuid+uid);
      uint64_t t_start = timestamp();
      status = execute(ch, url); 
      if ( status < 0 ) { WHEREAMI; ptr_tinfo->status = -1; return NULL; }
      uint64_t t_stop = timestamp();
      if ( t_stop > t_start ) { 
        ptr_tinfo->time_taken += (t_stop - t_start);
        num_hits++;
      }
    }
  }
  ptr_tinfo->num_hits = num_hits;
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
  long http_code;
  double c_length;  
  FILE *fp = NULL;
#define MAX_LEN_SERVER_NAME 511
  char server[MAX_LEN_SERVER_NAME+1];
  // char *url = "http://cinco.corp.linkedin.com";
  // char *url = "http://www.google.com";
  char url[1024];
  uint64_t t_start, t_stop;
  pthread_t *threads = NULL;
  THREAD_INFO_TYPE *tinfo = NULL;

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

  memset(g_chunk, '\0', CHUNK_SIZE);

  int num_tests   = 64;
  int num_threads = 128;
  // Create as many threads as tests
  threads = malloc(num_threads * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  tinfo = malloc(num_threads * sizeof(THREAD_INFO_TYPE));
  return_if_malloc_failed(tinfo);
  int nU = 4096; // TODO FIX = 524288; Set to half of sz_uuid_ht in ab.test.conf
  int niter = 1;
  int start_uuid = 12345678;
  char buf[1024];

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
    curl_easy_setopt(pch[tid], CURLOPT_WRITEDATA, (void *)&g_chunk);
  }

  // Set base URL
  memset(g_base_url, '\0', 1024);
  sprintf(g_base_url, "%s:%d", server, ab_port);
  // Restart the log server

  sprintf(url, "http://%s:%d/Restart", server, log_port);
  fprintf(stderr, "url = %s \n" , url);
  status = execute(pch[0], url); cBYE(status);


#undef SEQUENTIAL
  for ( int tid = 0; tid < num_threads; tid++ ) { 
    tinfo[tid].tid        = tid;
    tinfo[tid].status     = 0;
    tinfo[tid].num_tests  = num_tests;
    tinfo[tid].time_taken = 0;
    tinfo[tid].nU         = nU;
    tinfo[tid].start_uuid = start_uuid;
    tinfo[tid].niter      = niter;
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
    total_hits += tinfo[tid].num_hits;
  }
  fprintf(stderr, "Time/hit = %lf \n", (total_time/1000.0)/total_hits);
BYE:
  if ( pch != NULL ) { 
    for ( int tid = 0; tid < num_threads; tid++ ) { 
      if ( pch[tid] != NULL ) { 
        curl_easy_cleanup(pch[tid]); 
        pch[tid] = NULL;
      }
    }
    curl_global_cleanup();
  }
  free_if_non_null(threads);
  free_if_non_null(tinfo);
  fclose_if_non_null(fp);
  return status ;
}

#include <pthread.h>
#include <limits.h>
#include <float.h>
#include "ab_incs.h"
#include "auxil.h"
#include "mmap.h"
#include "setup_curl.h"
#include "execute.h"

typedef struct _thread_info_type {
  uint64_t time_taken;
  int num_good;
  int num_bad;
  double avg_time;
  double min_time;
  double max_time;
} THREAD_INFO_TYPE;

CURL **g_ch; // [nT]
char *g_server;
int32_t g_port;
int g_num_iters; 
int g_num_threads;
char *g_payload;
THREAD_INFO_TYPE *g_tinfo;


extern void *
hammer(
    void *arg
    );

void *
hammer(
    void *arg
    )
{
  char url[1024];
  int tid = *((int *)arg);
  int num_bad = 0;
  int num_good = 0;
  long http_code = 0;
  uint64_t time_taken = 0;
  double avg_time = 0;
  double min_time = DBL_MAX;
  double max_time = -1.0 * DBL_MAX;
  uint64_t t, t1, t2;

  sprintf(url, "%s:%d/Classify", g_server, g_port);
  for ( int iter = 0; iter < g_num_iters; iter++ ) {
    t1 = RDTSC();
    execute(g_ch[tid], url, &http_code);
    t2 = RDTSC();
    if ( http_code != 200 ) { num_bad++; continue; }
    if ( t2 < t1 ) {  continue; }
    num_good++;
    t = t2 - t1;
    time_taken += t;
    if ( t > max_time ) { max_time = t; }
    if ( t < min_time ) { min_time = t; }
    avg_time += t;
  }
  /* fprintf(stderr, "Hammer Finished %d \n", tid);
  fprintf(stderr, "%d: %d, %d, %lf \n", tid, num_bad, num_good, 
      (double)time_taken);
  */
  g_tinfo[tid].num_bad    = num_bad;
  g_tinfo[tid].num_good   = num_good;
  g_tinfo[tid].time_taken = time_taken;
  g_tinfo[tid].avg_time   = avg_time / (double)num_good;
  g_tinfo[tid].min_time = min_time;
  g_tinfo[tid].max_time = max_time;
  return NULL;
}

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  g_tinfo = NULL;
  g_payload = NULL;
  char *X = NULL; size_t nX = 0;
  pthread_t *threads = NULL;
  g_ch = NULL;
  int *tids = NULL;
  g_server = strdup("localhost");
  g_port = 8000;
  if ( argc != 4 ) { 
    fprintf(stderr, 
        "Usage is %s: <num_threads> <num_iterations> <payload_file>\n", 
        argv[0]);
    go_BYE(-1); 
  }
  status = stoI4(argv[1], &g_num_threads); cBYE(status);
  status = stoI4(argv[2], &g_num_iters); cBYE(status);
  // Create payload for POST
  char *post_file_name = argv[3];
  status = rs_mmap(post_file_name, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  if ( X  == NULL ) { go_BYE(-1); }
  g_payload = malloc(nX+1);
  memset(g_payload, '\0', nX+1);
  memcpy(g_payload, X, nX);

  // info collected by each thread
  g_tinfo = malloc(g_num_threads * sizeof(THREAD_INFO_TYPE));
  return_if_malloc_failed(g_tinfo);
  memset(g_tinfo, '\0',  g_num_threads * sizeof(THREAD_INFO_TYPE));


  tids = malloc(g_num_threads * sizeof(int));
  return_if_malloc_failed(tids);
  for ( int i = 0; i < g_num_threads; i++ ) {  tids[i] = i; }

  g_ch = malloc(g_num_threads * sizeof(CURL *));
  return_if_malloc_failed(g_ch);
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    status = setup_curl(&(g_ch[tid])); cBYE(status);
    /* Now specify the POST data */ 
    curl_easy_setopt(g_ch[tid], CURLOPT_POSTFIELDS, g_payload);
  }

  threads = malloc(g_num_threads * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  memset(threads, '\0', g_num_threads * sizeof(pthread_t));

  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    pthread_create(&(threads[tid]), NULL, hammer, (void *)(tids+tid));
  }
  fprintf(stderr, "forked all threads\n");
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    pthread_join(threads[tid], NULL);
  }
  fprintf(stderr, "joined all threads\n");
  uint64_t time_taken = 0;
  uint64_t num_good = 0;
  uint64_t num_bad = 0;
  for ( int tid = 0; tid < g_num_threads; tid++ ) { 
    time_taken += g_tinfo[tid].time_taken;
    num_good   += g_tinfo[tid].num_good;
    num_bad    += g_tinfo[tid].num_bad;
  }
  double ghz = 2500.0 ;
  fprintf(stderr, "Number of failures    = %d \n", (int)num_bad);
  fprintf(stderr, "Total Number of  hits = %d \n", (int)num_good);
  fprintf(stderr, "Average Time per hit  = %lf \n", 
      (double)time_taken/( ghz * (double)num_good));
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
  free_if_non_null(tids);
  free_if_non_null(g_tinfo);
  free_if_non_null(g_payload);
  rs_munmap(X, nX);
  return status ;
}

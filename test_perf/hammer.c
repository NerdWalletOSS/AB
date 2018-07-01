#include "ab_incs.h"
#include <float.h>
#include "macros.h"
#include "execute.h"
#include "auxil.h"
#include "hammer.h"
#include "read_test_info.h"
#include "make_guid.h"
extern CURL **g_ch;
extern int g_num_users;
extern int g_num_iters;
extern char *g_server;
extern int32_t g_port;
extern TEST_INFO_REC_TYPE *g_T; /* [g_num_tests] */
extern int g_num_tests;

void *
hammer(
    void *arg
    )
{
  int status = 0;
  int tid = *((int *)arg);
  int ndots = 0;
  int start_uuid = 1000000*tid;
  int hit_ctr = 0;
  int num_bad = 0;
  long http_code = 0;
  double avg_time = 0;
  double min_time = DBL_MAX;
  double max_time = -1.0 * DBL_MAX;

  fprintf(stderr, "Hammer started %d \n", tid);
  for ( int iter = 0; iter < g_num_iters; iter++ ) {
    // T is used to measure individual times 
    for ( int uid = 0; uid < g_num_users; uid++ ) {
      for ( int test_id = 0; test_id < g_num_tests; test_id++ ) { 
        char url[AB_MAX_LEN_URL+1];
        char tracer[AB_MAX_LEN_TRACER+1];
        for ( int i = 0; i < AB_MAX_LEN_URL+1; i++ ) { url[i] = '\0'; }
        for ( int i = 0; i < AB_MAX_LEN_TRACER+1; i++ ) { tracer[i] = '\0';}
        status = make_guid(NULL, tracer, AB_MAX_LEN_TRACER); 
        cBYE(status);
        sprintf(url, "%s:%d/GetVariant?TestName=%s&TestType=%s&UUID=%d&Tracer=%s", 
            g_server, g_port,
            g_T[test_id].test_name, 
            g_T[test_id].test_type, 
            start_uuid+uid, tracer);
        uint64_t t1 = get_time_usec();
        // fprintf(stderr, "%s\n", url);
        execute(g_ch[tid], url, &http_code);
        if ( http_code != 200 ) { continue; num_bad++; }
        uint64_t t2 = get_time_usec();
        if ( t2 < t1 ) {  continue; }
        uint64_t t = t2 - t1;
        if ( t > max_time ) { max_time = t; }
        if ( t < min_time ) { min_time = t; }
        avg_time += t;
        hit_ctr++;
        t = t % 1000; // convert to msec
        if ( ( hit_ctr % 50 ) == 0 ) {
          sprintf(url, "%s:%d/Diagnostics?Source=C", g_server, g_port);
          status = execute(g_ch[tid], url, &http_code); cBYE(status);
          if ( http_code != 200 ) { go_BYE(-1); }
          ndots++; 
          if ( tid == 0 ) { 
            fprintf(stderr, ".");
            if ( ndots >= 64 ) { 
              fprintf(stderr, "%d\n", hit_ctr);
              ndots = 0;
            }
          }
        }
      }
    }
  }
BYE:
  fprintf(stderr, "Hammer Finished %d \n", tid);
  return NULL;
}

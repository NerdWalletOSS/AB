#include "dt_incs.h"
#include "read_random_forest.h"
#include "txt_to_F4.h"
#include "eval_mdl.h"

uint64_t g_num_compares;
uint64_t g_num_models;
// START FUNC DECL
static uint64_t get_time_usec(
    void
    )
// STOP FUNC DECL
{
  struct timeval Tps;
  struct timezone Tpf;
  unsigned long long t = 0, t_sec = 0, t_usec = 0;

  gettimeofday (&Tps, &Tpf);
  t_sec  = (uint64_t )Tps.tv_sec;
  t_usec = (uint64_t )Tps.tv_usec;
  t = t_sec * 1000000 + t_usec;
  return t;
}


/* assembly code to read the TSC */
static uint64_t 
RDTSC(void)
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

DT_REC_TYPE *dt = NULL;
int n_dt = 0;
RF_REC_TYPE *rf = NULL;
int n_rf = 0;
MDL_REC_TYPE *mdl = NULL;
int n_mdl = 0;

static int
is_uq(
    int argc,
    char **argv
    )
{
  int status = 0;
  for ( int i = 0; i < argc; i++ ) { 
    for ( int j = i+1; i < argc; i++ ) { 
      if ( strcmp(argv[i], argv[j]) == 0 ) { go_BYE(-1);
      }
    }
  }
BYE:
  return status;
}
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *random_forest_file_name = NULL;
  char *test_data_file_name     = NULL;
  char *dt_file_name     = NULL;
  char *rf_file_name     = NULL;
  char *mdl_file_name     = NULL;
  float *invals = NULL;
  FILE *fp = NULL;
  FILE *ofp = NULL;
  if ( argc != 6 ) { go_BYE(-1); }
  random_forest_file_name = argv[1];
  test_data_file_name     = argv[2];
  dt_file_name            = argv[3];
  rf_file_name            = argv[4];
  mdl_file_name           = argv[5];

  status = is_uq(argv, argc); cBYE(status);

  g_num_compares = 0;

  status = read_random_forest(random_forest_file_name, 
      &dt, &n_dt, &rf, &n_rf, &mdl, &n_mdl); 
  cBYE(status);
  int nF = 0 ; 
  for ( int i = 0; i < n_dt; i++ ) { nF = max(nF, dt[i].feature_idx); }
  nF++; // this is important
  invals = malloc((nF+1) * sizeof(float)); // +1 for prediction
  return_if_malloc_failed(invals);
  fp = fopen(test_data_file_name, "r");
  return_if_fopen_failed(fp,  test_data_file_name, "r");
#define MAXLINE 65535
  char line [MAXLINE+1];
  int lno = 0;
  uint64_t sum1 = 0, sum2 = 0;
  int n_iters = 1; int n_trials = 0;
  for ( int iters = 0; iters < n_iters; iters++ ) {
    bool is_hdr = true;
    for ( lno = 0; !feof(fp); lno++) { 
      memset(line, '\0', MAXLINE+1);
      char *cptr = fgets(line, MAXLINE, fp);
      if ( cptr == NULL ) { break; }
      if ( is_hdr ) { is_hdr = false; continue; } // skip header line
      if ( feof(fp) ) { break; }
      for ( uint32_t i = 0; i < strlen(line); i++ ) { 
        if ( line[i] == '\n' ) { line[i] = '\0' ; }
      }
      for ( int i = 0; i < nF+1; i++ ) { 
        char *xptr;
        if ( i == 0 ) { 
          xptr = strtok(line, ","); 
        } 
        else { 
          xptr = strtok(NULL, ",");
        }
        status = txt_to_F4(xptr, &(invals[i])); 
        if ( status < 0 ) { 
          printf("hello world\n");
        }
        cBYE(status);
      }
      uint64_t t1a = RDTSC(); 
      uint64_t t2a = get_time_usec();
      status = eval_mdl(invals, nF, dt, n_dt, rf, n_rf, mdl, n_mdl); 
      uint64_t t1b = RDTSC(); 
      uint64_t t2b = get_time_usec();
      uint64_t d1 = (t1b - t1a);
      uint64_t d2 = (t2b - t2a);
      sum1 += d1;
      sum2 += d2;
      n_trials++;
      // printf("%d, %d, d1 = %llu, d2 = %llu \n", iters, n_trials, d1, d2);
      cBYE(status);
    }
    fclose_if_non_null(fp);
    fp = fopen(test_data_file_name, "r");
    return_if_fopen_failed(fp,  test_data_file_name, "r");
    is_hdr = true;
  }
  lno--; // remove header line 
  printf("Number of trials = %d \n", n_trials);
  printf("Number of random forests = %d \n", g_num_models);
  printf("Number of compares = %d \n", g_num_compares);
  printf("#Test = %d, time = %lf\n", lno, sum1 / (double)n_trials);
  printf("#Test = %d, time = %lf\n", lno, sum2 / (double)n_trials);
  printf("Total time (1, 2) = %llu %llu \n", sum1, sum2);
  printf("COMPLETED\n");
  //---------------------------------------------------
  ofp = fopen(dt_file_name, "wb");
  return_if_fopen_failed(ofp, dt_file_name, "wb");
  fwrite(dt, sizeof(DT_REC_TYPE), n_dt, ofp);
  fclose_if_non_null(ofp);
  //---------------------------------------------------
  ofp = fopen(rf_file_name, "wb");
  return_if_fopen_failed(ofp, rf_file_name, "wb");
  fwrite(rf, sizeof(RF_REC_TYPE), n_rf, ofp);
  fclose_if_non_null(ofp);
  //---------------------------------------------------
  ofp = fopen(mdl_file_name, "wb");
  return_if_fopen_failed(ofp, mdl_file_name, "wb");
  fwrite(mdl, sizeof(MDL_REC_TYPE), n_mdl, ofp);
  fclose_if_non_null(ofp);

BYE:
  fclose_if_non_null(ofp);
  fclose_if_non_null(fp);
  free_if_non_null(dt);
  free_if_non_null(rf);
  free_if_non_null(mdl);
  free_if_non_null(invals);
  return status;
}

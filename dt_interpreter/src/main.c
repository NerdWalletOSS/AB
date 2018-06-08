#include "dt_incs.h"
#include "read_random_forest.h"
#include "txt_to_F4.h"
#include "eval_mdl.h"
#include "auxil.h"

uint64_t g_num_compares;

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
    for ( int j = i+1; j < argc; j++ ) { 
      if ( strcmp(argv[i], argv[j]) == 0 ) { 
        printf("hello world\n"); 
        fprintf(stderr, "Arguments %d and %d are same = %s\n", 
            i, j, argv[i]);
        go_BYE(-1);
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
  float *predictions = NULL;
  if ( ( argc != 5 )  && ( argc != 6 ) ) { go_BYE(-1); }
  random_forest_file_name = argv[1];
  dt_file_name            = argv[2];
  rf_file_name            = argv[3];
  mdl_file_name           = argv[4];
  if ( argc == 6 ) { 
    test_data_file_name     = argv[5];
  }

  status = is_uq(argc, argv); cBYE(status);

  g_num_compares = 0;

  status = read_random_forest(random_forest_file_name, 
      &dt, &n_dt, &rf, &n_rf, &mdl, &n_mdl); 
  cBYE(status);
  predictions = malloc(n_mdl * sizeof(float));
  return_if_malloc_failed(predictions);
  //  Write binary output files 
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

  if ( test_data_file_name == NULL ) { 
    fprintf(stderr, "NO test data provided. Quitting early\n"); goto BYE;
  }
  int nF = 0 ; 
  for ( int i = 0; i < n_dt; i++ ) { nF = max(nF, dt[i].feature_idx); }
  nF++; // this is important
  invals = malloc((nF+1) * sizeof(float)); // +1 for prediction
  return_if_malloc_failed(invals);
  fp = fopen(test_data_file_name, "r");
  return_if_fopen_failed(fp,  test_data_file_name, "r");
  // TODO P1 Need to be clear on what test_data_file looks like
#define MAXLINE 65535
  char line [MAXLINE+1];
  int lno = 0;
  uint64_t sum1 = 0, sum2 = 0;
  int n_iters = 1; int n_trials = 0;
  n_iters = 1;
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
      for ( int i = 0; i < nF; i++ ) { 
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
      uint64_t t2a = get_time_usec();
      uint64_t t1a = RDTSC(); 
      status = eval_mdl(invals, nF, dt, n_dt, rf, n_rf, mdl, n_mdl,
          predictions);
      uint64_t t1b = RDTSC(); 
      uint64_t t2b = get_time_usec();
      uint64_t d1 = (t1b - t1a);
      uint64_t d2 = (t2b - t2a);
      sum1 += d1;
      sum2 += d2;
      n_trials++;
      printf("%d, %d, d1 = %llu, d2 = %llu \n", iters, n_trials, d1, d2);
      cBYE(status);
    }
    fclose_if_non_null(fp);
    fp = fopen(test_data_file_name, "r");
    return_if_fopen_failed(fp,  test_data_file_name, "r");
    is_hdr = true;
  }
  lno--; // remove header line 
  printf("Number of trials = %d \n", n_trials);
  printf("Number of compares = %d \n", g_num_compares);
  printf("#Test = %d, cycles = %lf\n", lno, sum1 / (double)n_trials);
  printf("#Test = %d, time = %lf\n", lno, sum2 / (double)n_trials);
  printf("COMPLETED\n");

BYE:
  fclose_if_non_null(ofp);
  fclose_if_non_null(fp);
  free_if_non_null(dt);
  free_if_non_null(rf);
  free_if_non_null(mdl);
  free_if_non_null(invals);
  free_if_non_null(predictions);
  return status;
}

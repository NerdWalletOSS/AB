#include "dt_incs.h"
#include "read_random_forest.h"
#include "txt_to_F4.h"

DT_REC_TYPE *dt = NULL;
int n_dt = 0;
RF_REC_TYPE *rf = NULL;
int n_rf = 0;

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *random_forest_file_name = NULL;
  char *test_data_file_name     = NULL;
  float *invals = NULL;
  FILE *fp = NULL;
  if ( argc != 3 ) { go_BYE(-1); }
  random_forest_file_name = argv[1];
  test_data_file_name     = argv[2];

  status = read_random_forest(random_forest_file_name, 
      &dt, &n_dt, &rf, &n_rf); 
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
  for ( ; !feof(fp); lno++) { 
    memset(line, '\0', MAXLINE+1);
    char *cptr = fgets(line, MAXLINE, fp);
    if ( cptr == NULL ) { break; }
    if ( lno == 0 ) { continue; } // skip header line
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
      cBYE(status);
    }
    status = eval_rf(invals, nF, dt, n_dt, rf, n_rf);
    cBYE(status);
  }
  printf("COMPLETED\n");
  
BYE:
  fclose_if_non_null(fp);
  free_if_non_null(dt);
  free_if_non_null(rf);
  free_if_non_null(invals);
  return status;
}

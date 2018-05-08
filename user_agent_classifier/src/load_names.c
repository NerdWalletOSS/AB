#include "ua_incs.h"
#include "load_names.h"
#include "auxil.h"

int
load_names(
    const char *names_file,
    MODEL_NAME_TYPE **ptr_M,
    int *ptr_nM
    )
{
  int status = 0;
  MODEL_NAME_TYPE *M = NULL; int nM = 0;
  FILE *fp = NULL;
  char *buf = NULL;
  int buflen = 1024;
  buf = malloc(buflen);
  return_if_malloc_failed(buf);
  // get nM
  status = num_lines(names_file, &nM); cBYE(status);
  // malloc and initialize data structure
  M = malloc(nM * sizeof(MODEL_NAME_TYPE));
  return_if_malloc_failed(M);
  for ( int i = 0; i < nM; i++ ) { 
    M[i].intercept = 0;
    memset(M[i].model, '\0', UA_MAX_LEN_MODEL_NAME+1);
  }
  // load from file 
  fp = fopen(names_file, "r");
  return_if_fopen_failed(fp, names_file, "r");
  for ( int i = 0; i < nM; i++ ) { 
    if ( feof(fp) ) { go_BYE(-1); }
    memset(buf, '\0', buflen);
    char *rslt = fgets(buf, buflen, fp);
    if ( rslt == NULL ) { go_BYE(-1); }
    // Note assumption that comma is not in word
    char *word = strtok(rslt, ","); 
    if ( strlen(word) > UA_MAX_LEN_MODEL_NAME ) { go_BYE(-1); } 
    strcpy(M[i].model, word);
    char *endptr = NULL;
    char *str_val = strtok(NULL, ",");
    if ( ( str_val == NULL ) || ( *str_val == '\0' ) ) { go_BYE(-1); }
    float fval= strtof(str_val, &endptr);
    M[i].intercept = fval;
  }
  // ---------------
  *ptr_M = M;
  *ptr_nM = nM;
BYE:
  if ( status < 0 ) { 
    *ptr_M = NULL; *ptr_nM = 0; 
    free_if_non_null(M); 
  }
  free_if_non_null(buf); 
  fclose_if_non_null(fp);
  return status;
}

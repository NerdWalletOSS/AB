#include "ua_incs.h"
#include "load_model.h"
#include "auxil.h"

int
load_model(
    const char *model_file,
    int num_models,
    MODEL_REC_TYPE **ptr_M,
    int *ptr_nM
    )
{
  int status = 0;
  MODEL_REC_TYPE *M = NULL; int nM = 0;
  FILE *fp = NULL;
  char *buf = NULL;
  int buflen = UA_MAX_LEN_WORD+1+(num_models*32);
  buf = malloc(buflen);
  return_if_malloc_failed(buf);
  // get nM
  status = num_lines(model_file, &nM); cBYE(status);
  M = malloc(nM * sizeof(MODEL_REC_TYPE));
  return_if_malloc_failed(M);
  for ( int i = 0; i < nM; i++ ) { 
    M[i].coefficients = NULL;
    memset(M[i].word, '\0', UA_MAX_LEN_WORD+1);
  }
  for ( int i = 0; i < nM; i++ ) { 
    M[i].coefficients = malloc(num_models * sizeof(float));
    return_if_malloc_failed(M[i].coefficients);
  }
  fp = fopen(model_file, "r");
  return_if_fopen_failed(fp, model_file, "r");
  for ( int i = 0; i < nM; i++ ) { 
    if ( feof(fp) ) { go_BYE(-1); }
    memset(buf, '\0', buflen);
    char *rslt = fgets(buf, buflen, fp);
    if ( rslt == NULL ) { go_BYE(-1); }
    // Note assumption that comma is not in word
    char *word = strtok(rslt, ","); 
    if ( strlen(word) > UA_MAX_LEN_WORD ) { 
      printf("hello world\n");
      go_BYE(-1); 
    } 
    strcpy(M[i].word, word);
    for ( int k = 0; k < num_models; k++ ) { 
      char *endptr = NULL;
      char *str_val = strtok(NULL, ",");
      if ( ( str_val == NULL ) || ( *str_val == '\0' ) ) { 
        printf("hello world\n");
        go_BYE(-1); 
      }
      float fval= strtof(str_val, &endptr);
      // TODO Any constraints on fval 
      M[i].coefficients[k] = fval;
    }
  }
  *ptr_M = M;
  *ptr_nM = nM;
BYE:
  if ( status < 0 ) { 
    *ptr_M = NULL; *ptr_nM = 0; 
    for ( int i = 0; i < nM; i++ ) { 
      free_if_non_null(M[i].coefficients);
    }
    free_if_non_null(M); 
  }
  free_if_non_null(buf); 
  fclose_if_non_null(fp);
  return status;
}

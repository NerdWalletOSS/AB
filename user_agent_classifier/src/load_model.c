#include "ua_incs.h"
#include "load_model.h"

int
load_model(
    const char *model_file,
    int num_models,
    MODEL_REC_TYPE **ptr_V,
    int *ptr_nV
    )
{
  int status = 0;
  MODEL_REC_TYPE *V = NULL; int nV = 0;
  FILE *fp = NULL;
  char *buf = NULL;
  int buflen = UA_MAX_LEN_WORD+1+(num_models*32);
  buf = malloc(buflen);
  return_if_malloc_failed(buf);
  // get nV
  V = malloc(nV * sizeof(MODEL_REC_TYPE));
  return_if_malloc_failed(V);
  fp = fopen(model_file, "r");
  return_if_fopen_failed(fp, model_file, "r");
  for ( int i = 0; i < nV; i++ ) { 
    if ( feof(fp) ) { go_BYE(-1); }
    memset(buf, '\0', buflen);
    char *rslt = fgets(buf, UA_MAX_LEN_WORD+1, fp);
    if ( rslt == NULL ) { go_BYE(-1); }
    // Note assumption that comma is not in word
    char *word = strtok(rslt, ","); 
    if ( strlen(word) > UA_MAX_LEN_WORD ) { go_BYE(-1); } 
    strcpy(V[i].word, word);
    for ( int k = 0; k < num_models; k++ ) { 
      char *endptr = NULL;
      char *str_val = strtok(NULL, ",");
      if ( ( str_val == NULL ) || ( *str_val == '\0' ) ) { go_BYE(-1); }
      float fval= strtof(str_val, &endptr);
      // TODO Any constraints on fval 
      V[i].coefficients[k] = fval;
    }
  }
  *ptr_V =V;
  *ptr_nV = nV;
BYE:
  if ( status < 0 ) { free_if_non_null(V); *ptr_V = NULL; *ptr_nV = 0; }
  fclose_if_non_null(fp);
  return status;
}

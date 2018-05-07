#include "ua_incs.h"
#include "load_model.h"
#include "score_url.h"

#define MAX_LEN_URL 1023
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *model_file_name     = NULL;
  char *test_file_name      = NULL;
  MODEL_REC_TYPE *M = NULL; int nM;
  FILE *fp = NULL;
  int num_models = 6;
  float *scores = NULL;

  if ( argc != 3 )  { go_BYE(-1); }
  model_file_name = argv[1];
  test_file_name  = argv[2];
  status = load_model(model_file_name, num_models, &M, &nM); cBYE(status);
  fprintf(stderr, "loaded %d rows \n", nM);
  //----- Start testing
  char url[MAX_LEN_URL+1];

  scores = malloc(num_models * sizeof(float));
  return_if_malloc_failed(scores);

  fp = fopen(test_file_name, "r");
  return_if_fopen_failed(fp, test_file_name, "r");
  int num_urls = 0;
  for ( int i = 0; i < nM; i++ ) { 
    if ( feof(fp) ) { go_BYE(-1); }
    memset(url, '\0', MAX_LEN_URL+1);
    char *xurl = fgets(url, MAX_LEN_URL, fp);
    if ( xurl == NULL ) { break; }
    status = score_url(url, scores, num_models, M, nM); cBYE(status);
    num_urls++;
  }
  printf("Read %d URLs \n", num_urls);
  
BYE:
  if ( M != NULL ) { 
    for ( int i = 0; i < nM; i++ ) { 
      free_if_non_null(M[i].coefficients);
    }
  }
  free_if_non_null(M); 
  fclose_if_non_null(fp);
  free_if_non_null(scores);
  return status;
}

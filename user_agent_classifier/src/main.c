#include "ua_incs.h"
#include "load_model.h"
#include "load_names.h"
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
  char *coeff_file_name     = NULL;
  char *test_file_name      = NULL;
  MODEL_REC_TYPE *M = NULL; int nM;
  MODEL_NAME_TYPE *N = NULL; int num_models;
  FILE *fp = NULL;
  float *scores = NULL;

  if ( argc != 4 )  { go_BYE(-1); }
  model_file_name = argv[1];
  coeff_file_name = argv[2];
  test_file_name  = argv[3];

  status = load_names(model_file_name, &N, &num_models); cBYE(status);
  status = load_model(coeff_file_name, num_models, &M, &nM); cBYE(status);
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
    int best_model = 0; float best_score = scores[0];
    for ( int m = 1; m < num_models; m++ ) { 
      if ( scores[m] > best_score ) { 
        best_score = scores[m];
        best_model = m;
      }
    }
    fprintf(stderr, "%s\n", N[best_model].model);
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
  free_if_non_null(N); 
  fclose_if_non_null(fp);
  free_if_non_null(scores);
  return status;
}

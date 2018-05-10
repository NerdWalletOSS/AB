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
  double *scores = NULL;
  uint64_t *H = NULL; int nH = 0; // hashes
  uint64_t total_time = 0;

  if ( argc != 4 )  { go_BYE(-1); }
  model_file_name = argv[1];
  coeff_file_name = argv[2];
  test_file_name  = argv[3];

  status = load_names(model_file_name, &N, &num_models); cBYE(status);
  status = load_model(coeff_file_name, num_models, &M, &nM, &H, &nH); 
  cBYE(status);
  fprintf(stderr, "loaded %d rows \n", nM);
  //----- Start testing
  char url[MAX_LEN_URL+1];

  scores = malloc(num_models * sizeof(double));
  return_if_malloc_failed(scores);

  fp = fopen(test_file_name, "r");
  return_if_fopen_failed(fp, test_file_name, "r");
  int num_urls = 0;
  for ( ; ; ) { 
    if ( feof(fp) ) { go_BYE(-1); }
    memset(url, '\0', MAX_LEN_URL+1);
    char *xurl = fgets(url, MAX_LEN_URL, fp);
    if ( xurl == NULL ) { break; }
    int best_model = -1;
    uint64_t t_start = get_time_usec();
    status = score_url(url, M, nM, N, num_models, &best_model); 
    uint64_t t_stop = get_time_usec();
    total_time += (t_stop - t_start);
    cBYE(status);
    fprintf(stdout, "%s\n", N[best_model].model);
    num_urls++;
  }
  fprintf(stderr, "Read %d URLs \n", num_urls);
  fprintf(stderr, "Average time = %lf \n", total_time/(double)num_urls);
  
BYE:
  if ( M != NULL ) { 
    for ( int i = 0; i < nM; i++ ) { 
      free_if_non_null(M[i].coefficients);
    }
  }
  free_if_non_null(M); 
  free_if_non_null(N); 
  fclose_if_non_null(fp);
  fclose_if_non_null(H);
  free_if_non_null(scores);
  return status;
}

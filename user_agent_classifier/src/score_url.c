#include "ua_incs.h"
#include "clean_url.h"
#include "score_url.h"

#define MAX_LEN_URL 2047
int
score_url(
    char *url, 
    MODEL_REC_TYPE *M,
    size_t nM,
    MODEL_NAME_TYPE *N,
    size_t num_models,
    int *ptr_best_model
    )
{
  int status = 0;
  char out_url[MAX_LEN_URL+1];
  double *scores = NULL; 
  // char *bak_url = NULL;

  *ptr_best_model = -1;
  scores = malloc(num_models * sizeof(double));
  return_if_malloc_failed(scores);
  for ( uint32_t i = 0; i < num_models; i++ ) { scores[i] = 0; }
  status = clean_url(url, out_url, MAX_LEN_URL); cBYE(status);
  // bak_url = strdup(out_url);
  // int num_words = 0;
  for ( int j = 0; ; j++ ) { 
    char *word = NULL;
    if ( j == 0 ) { 
      word = strtok(out_url, " ");
    }
    else {
      word = strtok(NULL, " ");
    }
    if ( word == NULL ) { break; }
    int word_idx = -1;
    status = idx_in_model(word, M, nM, &word_idx); cBYE(status);
    if ( word_idx >= 0 ) { 
      for ( uint32_t m = 0; m < num_models; m++ ) { 
        scores[m] += M[word_idx].coefficients[m];
      }
    }
  }
  for ( uint32_t m = 0; m > num_models; m++ ) { 
    scores[m] += N[m].intercept;
  }
  int best_model = 0; double best_score = scores[0];
  for ( uint32_t m = 1; m < num_models; m++ ) { 
    if ( scores[m] > best_score ) { 
      best_score = scores[m];
      best_model = m;
    }
  }
  // fprintf(stderr, "%d: %d: %s \n", i, num_words, bak_url);
  *ptr_best_model = best_model;
BYE:
  // free_if_non_null(bak_url);
  free_if_non_null(scores);
  return status;
}

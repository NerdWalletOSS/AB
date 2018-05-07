#include "ua_incs.h"
#include "clean_url.h"
#include "score_url.h"

#define MAX_LEN_URL 2047
int
score_url(
    char *url, 
    float *scores, 
    int num_models,
    MODEL_REC_TYPE *M,
    size_t nM
    )
{
  int status = 0;
  char out_url[MAX_LEN_URL+1];
  // char *bak_url = NULL;

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
      for ( int m = 0; m < num_models; m++ ) { 
        scores[m] += M[word_idx].coefficients[m];
      }
    }
  }
  // fprintf(stderr, "%d: %d: %s \n", i, num_words, bak_url);
BYE:
  // free_if_non_null(bak_url);
  return status;
}

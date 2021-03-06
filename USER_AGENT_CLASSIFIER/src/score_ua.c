#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include "ua_types.h"
#include "macros.h"
#include "auxil.h"
#include "clean_ua.h"
#include "score_ua.h"

#define MAX_LEN_USER_AGENT 2047
int
score_ua(
    char *ua, 
    MODEL_REC_TYPE *M,
    size_t nM,
    MODEL_NAME_TYPE *N,
    size_t num_models,
    int *ptr_best_model
    )
{
  int status = 0;
  char out_ua[MAX_LEN_USER_AGENT+1];
  double *scores = NULL; 
  // char *bak_ua = NULL;


  if ( ua        == NULL ) { go_BYE(-1); } 
  if ( M          == NULL ) { go_BYE(-1); }
  if ( nM         == 0 )    { go_BYE(-1); }
  if ( N          == NULL ) { go_BYE(-1); }
  if ( num_models == 0 )    { go_BYE(-1); }

  *ptr_best_model = -1;
  scores = malloc(num_models * sizeof(double));
  return_if_malloc_failed(scores);
  for ( uint32_t i = 0; i < nM; i++ ) { M[i].is_seen = false; }
  for ( uint32_t i = 0; i < num_models; i++ ) { scores[i] = 0; }
  status = clean_ua(ua, out_ua, MAX_LEN_USER_AGENT); cBYE(status);
  // printf("out = %s \n", out_ua);
  // bak_ua = strdup(out_ua);
  // int num_words = 0;
  for ( int j = 0; ; j++ ) { 
    char *word = NULL;
    if ( j == 0 ) { 
      word = strtok(out_ua, " ");
    }
    else {
      word = strtok(NULL, " ");
    }
    if ( word == NULL ) { break; }
    int word_idx = -1;
    status = idx_in_model(word, M, nM, &word_idx); cBYE(status);
    if ( word_idx >= 0 ) { 
      if ( !M[word_idx].is_seen ) { 
        for ( uint32_t m = 0; m < num_models; m++ ) { 
          scores[m] += M[word_idx].coefficients[m];
        }
        // printf("word %d = %s \n", j, word);
        M[word_idx].is_seen = true;
      }
    }
  }
  //---------------------------
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
  // fprintf(stderr, "%d: %d: %s \n", i, num_words, bak_ua);
  *ptr_best_model = best_model;
BYE:
  // free_if_non_null(bak_ua);
  free_if_non_null(scores);
  return status;
}

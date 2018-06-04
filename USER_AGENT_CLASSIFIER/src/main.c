#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "macros.h"
#include "ua_types.h"
#include "load_names.h"
#include "load_model.h"
#include "score_ua.h"
#include "classify_ua.h"
#include "auxil.h"

MODEL_REC_TYPE *g_ua_M; int g_n_ua_M;
MODEL_NAME_TYPE *g_ua_N; int g_ua_num_models;
uint64_t *g_ua_H; int g_n_ua_H;

#define MAXLINE 65535

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *fp = NULL;

  g_ua_M = NULL; g_n_ua_M = 0;
  g_ua_N = NULL; g_ua_num_models = 0;
  g_ua_H = NULL; g_n_ua_H = 0;

  if ( ( argc != 3 ) && ( argc != 4 ) ) { go_BYE(-1); }

  char *test_file = NULL;
  char *g_ua_model_coeff_file =  argv[1];
  char *g_ua_category_intercept_file = argv[2];
  if ( argc == 4 ) { 
    test_file = argv[3];
  }
  status = load_names(g_ua_category_intercept_file, &g_ua_N, &g_ua_num_models);
  status = load_model(g_ua_model_coeff_file, g_ua_num_models,
      &g_ua_M, &g_n_ua_M,  &g_ua_H, &g_n_ua_H);

  fp = fopen(test_file, "r");
  return_if_fopen_failed(fp, test_file, "r");
  for ( ; !feof(fp); ) {
    char user_agent[MAXLINE+1];
    memset(user_agent, '\0', MAXLINE+1);
    int best_model = -1;
    char *cptr = fgets(user_agent, MAXLINE, fp);
    if ( feof(fp) ) { break; }
    if ( cptr == NULL ) { break; }
    status = score_ua(user_agent, g_ua_M, g_n_ua_M, g_ua_N, g_ua_num_models, &best_model); 
    cBYE(status);
    if (( best_model < 0 )||( best_model >= g_ua_num_models ) ) { go_BYE(-1);}
    /*
    for ( unsigned int i = 0; i < g_n_devices; i++ ) { 
      if ( strcmp(g_N[best_model].model, g_devices[i].device) == 0 ) { 
        device_id = g_devices[i].device_id; break;
      }
    }
    */
  }
BYE:
  if ( g_ua_M != NULL ) { 
    for ( int i = 0; i <  g_n_ua_M; i++ ) { 
      free_if_non_null(g_ua_M[i].coefficients);
    }
  }
  free_if_non_null(g_ua_M);
  free_if_non_null(g_ua_N);
  free_if_non_null(g_ua_H);
  fclose_if_non_null(fp);
  return status;
}

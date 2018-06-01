#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "macros.h"
#include "ua_types.h"
#include "load_names.h"
#include "load_model.h"
#include "auxil.h"

MODEL_REC_TYPE *g_ua_M; int g_ua_nM;
MODEL_NAME_TYPE *g_ua_N; int g_ua_num_models;
uint64_t *g_ua_H; int g_ua_nH;

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  if ( argc != 3 ) { go_BYE(-1); }

  g_ua_M = NULL; g_ua_nM = 0;
  g_ua_N = NULL; g_ua_num_models = 0;
  g_ua_H = NULL; g_ua_nH = 0;

  char *g_ua_model_coeff_file =  argv[1];
  char *g_ua_category_intercept_file = argv[2];
  status = load_names(g_ua_category_intercept_file, &g_ua_N, &g_ua_num_models);
  status = load_model(g_ua_model_coeff_file, g_ua_num_models,
      &g_ua_M, &g_ua_nM,  &g_ua_H, &g_ua_nH);

BYE:
  if ( g_ua_M != NULL ) { 
    for ( int i = 0; i <  g_ua_nM; i++ ) { 
      free_if_non_null(g_ua_M[i].coefficients);
    }
  }
  free_if_non_null(g_ua_M);
  free_if_non_null(g_ua_N);
  free_if_non_null(g_ua_H);
  return status;
}

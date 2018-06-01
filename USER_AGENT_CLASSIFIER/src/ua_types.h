#ifndef __UA_TYPES_H
#define __UA_TYPES_H

#include "ua_consts.h"
typedef struct  _model_name_type { 
  char model[UA_MAX_LEN_MODEL_NAME+1];
  double intercept;
} MODEL_NAME_TYPE;

typedef struct  _model_rec_type { 
  bool is_seen;
  char word[UA_MAX_LEN_WORD+1];
  double *coefficients;
} MODEL_REC_TYPE;

#endif

#ifndef __DT_TYPES_H
#define __DT_TYPES_H

typedef struct _dt_rec_type { 
  int tree_id;
  int node_id;
  int left_id;
  int right_id;
  int feature_id;
  double threshold;
  int nneg;
  int npos;
} DT_REC_TYPE;
#endif

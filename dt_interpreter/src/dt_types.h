#ifndef __DT_TYPES_H
#define __DT_TYPES_H

typedef struct _dt_rec_type { 
  int tree_idx;
  int node_idx;
  int lchild_idx;
  int rchild_idx;
  int feature_idx;
  double threshold;
  int nneg;
  int npos;
} DT_REC_TYPE;
#endif

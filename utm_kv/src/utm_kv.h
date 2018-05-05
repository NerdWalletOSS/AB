#include "url.h"
#include "utm_incs.h"
#include "utm_types.h"
extern int
utm_kv(
    const char *instr,
    UTM_REC_TYPE *ptr_out
    );
extern void
free_utm_kv(
    UTM_REC_TYPE X
    );
extern int
print_utm_kv(
    UTM_REC_TYPE X,
    char *buf,
    size_t sz
    );

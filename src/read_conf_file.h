#include <jansson.h>
extern int
get_string(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    size_t maxlen,
    char *dst
    );
extern int
get_bool(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    bool *ptr_rslt
    );
extern  int
get_int(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    int32_t *ptr_rslt
    );
extern int
read_conf_file(
    const char *const file_name,
    CFG_TYPE *ptr_cfg
    );
extern int
validate_config(
    CFG_TYPE g_cfg
    );

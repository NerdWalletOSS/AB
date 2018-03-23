#include <stdlib.h>
extern char 
from_hex(
    char ch
    );
extern char 
to_hex(
    char code
    );
extern int 
url_encode(
    const char *in_str,
    char **ptr_out_str
    );
extern int 
url_decode(
    const char *in_str,
    char **ptr_out_str
    );
extern int
ext_url_encode(
    const char *in_str,
    char *rslt_buf,
    size_t sz_rslt_buf
    );
extern int
ext_url_decode(
    const char *in_str,
    char *rslt_buf,
    size_t sz_rslt_buf
    );

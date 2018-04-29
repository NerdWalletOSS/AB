#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "url.h"
#include "utm_kv.h"
extern UTM_REC_TYPE g_utm_kv;
extern char g_valid_chars_in_url[256];
extern char g_rslt[AB_MAX_LEN_RESULT+1];


//<hdr>
int
get_utm_kv(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  char *url = NULL;

  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  if ( strncmp(args, "URL=", 4) != 0 ) { go_BYE(-1); }
  status = url_decode(args+4, &url); cBYE(status);
  status = validate_chars(url, g_valid_chars_in_url); cBYE(status);
  status = utm_kv(url, &g_utm_kv); cBYE(status);;
  status = print_utm_kv(g_utm_kv, g_rslt, AB_MAX_LEN_RESULT); cBYE(status);
BYE:
  free_if_non_null(url);
  return status;
}

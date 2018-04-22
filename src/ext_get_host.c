#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "ab_auxil.h"
#include "url.h"
#include "ext_get_host.h"


//<hdr>
int
ext_get_host(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  int n = 1023;
  char in[n+1];
  char host[AB_MAX_LEN_HOSTNAME+1];
  char *alt_in = NULL;

  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  memset(in, '\0', n+1);
  memset(host, '\0', AB_MAX_LEN_HOSTNAME+1);
  status = extract_name_value(args, "URL=", '&', in, n);
  if ( in[0] == '\0' ) { go_BYE(-1); }
  status = url_decode(in, &alt_in); cBYE(status);
  status = get_host(alt_in, host, AB_MAX_LEN_HOSTNAME); cBYE(status);
  sprintf(X, " { \"Host\" : \"%s\" } ", host);
BYE:
  free_if_non_null(alt_in);
  return status;
}

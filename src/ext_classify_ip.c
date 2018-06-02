#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "ext_classify_ip.h"
#include "classify_ip.h"
#include "load_lkp.h"

//<hdr>
int
ext_classify_ip(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  char ip_address[AB_MAX_LEN_IP_ADDRESS+1];

  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  memset(ip_address, '\0', AB_MAX_LEN_IP_ADDRESS+1);
  status = extract_name_value(args, "IPAddress=", '&', 
      ip_address, AB_MAX_LEN_IP_ADDRESS);
  if ( ip_address[0] == '\0' ) { go_BYE(-1); }
  status = classify_ip(ip_address, &g_maxmind); cBYE(status);
  snprintf(X, nX, "{ \"PostalCode\" : \"%s\", \
                    \"TimeZone\" : \"%s\", \
                    \"Country\" : \"%s\", \
                    \"State\" : \"%s\", \
                    \"City\" : \"%s\" } ",
                    g_maxmind.postal_code,
                    g_maxmind.time_zone,
                    g_maxmind.country,
                    g_maxmind.state,
                    g_maxmind.city
      );
BYE:
  return status;
}

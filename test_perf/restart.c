#include "incs.h"
#include "ab_constants.h"
#include "macros.h"
#include "execute.h"
#include "restart.h"

int
restart(
    CURL *ch,
    const char *server,
    int port,
    const char *api
    )
{
  int status = 0;
  char url[1024];
  long http_code;
  fprintf(stderr, "Restarting the %s:%d server\n", server, port);
  curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, 1000);
  sprintf(url, "%s:%d/%s", server, port, api);
  status = execute(ch, url, &http_code); cBYE(status);
  if ( http_code == 0 ) { 
    fprintf(stderr, "Server %s:%d busy restarting\n", server, port); goto BYE;
  }
  if ( http_code != 200 ) { go_BYE(-1); }
BYE:
  return status;
}

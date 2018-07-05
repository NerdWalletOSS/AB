#include "incs.h"
#include "ab_constants.h"
#include "macros.h"
#include "add_tests.h"
#include "setup_curl.h"
#include "auxil.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *server = NULL;
  char *op_file_name = NULL;
  CURL *ch = NULL;
  int itemp;

  // Set globals 
  // process input parameters
  if ( argc != 5 ) { go_BYE(-1); }
  server       = argv[1];
  op_file_name = argv[4];

  status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp < 80 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  uint16_t port = (uint16_t)itemp; 

  status = stoI4(argv[3], &itemp); cBYE(status);
  if ( ( itemp <= 0 ) || ( itemp >= 1024 ) ) { go_BYE(-1); }
  uint16_t num_tests = (uint16_t)itemp; 
  //----------------------------------------
  status = setup_curl(&ch); cBYE(status);
  curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, 1000); 
  status = add_tests(ch, server, port, num_tests, op_file_name);
  cBYE(status);
BYE:
  if ( ch != NULL ) { curl_easy_cleanup(ch); }
  return status ;
}

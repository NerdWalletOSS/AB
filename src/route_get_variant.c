#include "ab_incs.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "get_variant.h"
#include "get_variants.h"
#include "auxil.h"
#include "alt_get_variant.h"
#include "route_get_variant.h"

//<hdr>
int
route_get_variant(
    AB_REQ_TYPE req_type,
    const char *args
    )
//</hdr>
{
  int status = 0;
  int test_type = -1;
  char *esc_args = NULL; int len_esc_args = 0;

  if ( ( args == NULL ) || ( *args == '\0' ) ) { go_BYE(-1); }
  esc_args = curl_easy_unescape(NULL, args, strlen(args), &len_esc_args);
  for ( char *cptr = esc_args; *cptr != '\0'; cptr++ ) { 
    if ( !isascii(*cptr) ) { go_BYE(-1); }
  }
  // status = validate_url_chars(esc_args); cBYE(status);

  switch ( req_type ) {
    case GetVariant  : 
      status = get_test_type(esc_args, &test_type);  cBYE(status);
      cBYE(status);
      switch ( test_type ) { 
        case AB_TEST_TYPE_AB : 
          status = get_variant(esc_args);  cBYE(status);
          break;
        case AB_TEST_TYPE_XY : 
          status = alt_get_variant(esc_args); cBYE(status);
          break;
        default : go_BYE(-1); break; 
      }
      cBYE(status);
      break;
    case GetVariants : 
      status = get_variants(esc_args);  cBYE(status); 
      break;
    default : go_BYE(-1); break;
  }
BYE:
  if ( esc_args != NULL ) { curl_free(esc_args); }
  return status;
}


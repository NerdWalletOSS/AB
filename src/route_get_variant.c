#include "ab_incs.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "get_variant.h"
#include "get_variants.h"
#include "auxil.h"
#include "alt_get_variant.h"
#include "route_get_variant.h"
#include "statsd.h"

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

  if ( ( args == NULL ) || ( *args == '\0' ) ) { go_BYE(-1); }
  for ( const char *cptr = args; *cptr != '\0'; cptr++ ) { 
    if ( !g_valid_chars_in_ab_args[(uint8_t)*cptr] ) { 
      g_log_bad_ab_args++; 
      STATSD_COUNT("bad_ab_args", 1); go_BYE(-1);
    }
  }

  switch ( req_type ) {
    case GetVariant  : 
      status = get_test_type(args, &test_type);  cBYE(status);
      cBYE(status);
      switch ( test_type ) { 
        case AB_TEST_TYPE_AB : 
          status = get_variant(args);  cBYE(status);
          break;
        case AB_TEST_TYPE_XY : 
          status = alt_get_variant(args); cBYE(status);
          break;
        default : go_BYE(-1); break; 
      }
      cBYE(status);
      break;
    case GetVariants : 
      status = get_variants(args);  cBYE(status); 
      break;
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}

#include "ab_incs.h"
#include "auxil.h"
#include "aux_zero.h"
#include "update_test.h"
#include <jansson.h>
#include "read_conf_file.h"
#include "get_test_idx.h"
#include "add_fake_test.h"

extern TEST_META_TYPE g_tests[AB_MAX_NUM_TESTS];

int
update_test(
    int test_idx,
    const char *const test_name,
    int test_type,
    int state,
    int is_dev_specific,
    const char *const args
    )
{
  int status = 0;
  // read buffer into JSON
  json_t *root = NULL;
  json_error_t error;
  root = json_loads(args, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }

  json_decref(root);
BYE:
  return status;
}

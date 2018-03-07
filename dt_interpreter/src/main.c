#include "dt_incs.h"
#include "read_random_forest.h"

DT_REC_TYPE *dt = NULL;
int n_dt = 0;

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *random_forest_file_name = NULL;
  char *test_data_file_name     = NULL;
  if ( argc != 3 ) { go_BYE(-1); }
  random_forest_file_name = argv[1];
  test_data_file_name     = argv[2];

  status = read_random_forest(random_forest_file_name, &dt, &n_dt);
  cBYE(status);
BYE:
  free_if_non_null(dt);
  return status;
}

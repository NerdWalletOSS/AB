#include "ab_incs.h"
#include "macros.h"
#include "auxil.h"
#include "read_test_info.h"

int
read_test_info(
    const char *file_name,
    TEST_INFO_REC_TYPE **ptr_T,
    int *ptr_num_T
    )
{
  int status = 0;
  TEST_INFO_REC_TYPE *T = NULL;
  int num_T = 0;
  FILE *fp = NULL;
#define MAXLINE 65535
  char line[MAXLINE+1];
  char *cptr, *xptr;

  status = num_lines(file_name, &num_T); cBYE(status);
  T = malloc(num_T * sizeof(TEST_INFO_REC_TYPE));
  return_if_malloc_failed(T);
  fp = fopen(file_name, "r");
  return_if_fopen_failed(fp,  file_name, "r");
  for ( int i = 0; i < num_T; i++ ) { 
    if ( feof(fp) ) { go_BYE(-1); }
     cptr = fgets(line, MAXLINE, fp);
     if ( cptr == NULL ) { go_BYE(-1); }
     cptr = strtok(line, ",");
     strcpy(T[i].test_name, cptr);

     xptr = strtok(NULL, ",");
     strcpy(T[i].test_type, xptr);

     xptr = strtok(NULL, ",");
     strcpy(T[i].state, xptr);
     
     xptr = strtok(NULL, ",");
     status = stoI4(xptr, &(T[i].is_dev_specific)); cBYE(status); 

     xptr = strtok(NULL, ",");
     status = stoI4(xptr, &(T[i].num_variants)); cBYE(status); 
  }
  *ptr_T = T;
  *ptr_num_T = num_T;
BYE:
  fclose_if_non_null(fp);
  return status;
}

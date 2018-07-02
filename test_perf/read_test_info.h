typedef struct _test_info_rec_type { 
  char test_name[AB_MAX_LEN_TEST_NAME+1];
  char test_type[16];
  char state[16];
  int is_dev_specific;
  int num_variants;
} TEST_INFO_REC_TYPE;

extern int
read_test_info(
    const char *file_name,
    TEST_INFO_REC_TYPE **ptr_T,
    int *ptr_num_T
    );

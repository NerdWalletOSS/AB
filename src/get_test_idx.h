extern int get_test_idx(
    const char *name,
    int test_type,
    int *ptr_test_idx
    );
extern int get_state_enum(
    const char *state,
    int *ptr_state_enum
    );
extern int 
get_test_type_enum(
    const char *test_type,
    int *ptr_test_type_enum
    );
extern int 
get_test_type(
      const char *args, 
      int *ptr_test_type
      );
extern int 
get_test_name(
    const char *args, 
    char *test_name
    );
extern int
get_test_from_args(
    const char *args,
    int *ptr_test_idx,
    int *ptr_test_type
    );

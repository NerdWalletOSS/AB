extern int 
chk_test_name(
    const char *X
    );
extern int 
chk_uuid(
    char *X,
    int desired_len
    );
extern int
add_to_buf(
    char *in,
    const char *label,
    char *out,
    int sz_out,
    int *ptr_n_out
    );
extern int
mk_json_output(
    char *api, 
    char *args, 
    char *err, 
    char *out
    );
extern void
get_tracer(
      const char *args, 
      char *tracer
      );
extern void
set_tracer(
    char *tracer,
    size_t n
    );
extern int 
get_host(
    char *in,
    char *out,
    int n_out
    );
extern int 
get_domain(
    const char *in,
    char *out,
    int n_out
    );

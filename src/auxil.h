extern int
stoF4(
      const char *X,
      float *ptr_valF4
      );
extern int
stoI4(
      const char *X,
      int32_t *ptr_Y
      );
extern int
stoI8(
      const char *X,
      int64_t *ptr_Y
      );
extern int
read_nth_val( /* n starts from 0 */
	     const char *in_str,
	     const char *delim,
	     int n,
	     char *out_str,
	     int len_out_str
	      );
extern int
extract_name_value(
		   const char *in_str,
		   const char *start,
		   char delim,
                   char *val, /* [len] */
                   int maxlen
		   );
extern int
strip_trailing_slash(
		     const char *in_str, 
		     char *out_str,
		     int out_len
		     );
extern int
count_char(
	   const char *X,
	   char c,
	   int *ptr_n
	   );
extern int
strip_extra_spaces(
		   char *X
		   );
extern uint32_t 
get_time_sec(
    void
    );
extern uint64_t 
get_time_usec(
    void
    );
extern void
zero_string_to_nullc(
		     char *X
		     );
extern unsigned int
prime_geq(
	  int n
	  );
extern bool
is_prime(
	 unsigned int n
	 );
extern int
extract_S(
    const char *X,
    const char *start_str,
    const char *stop_str,
    char *output,
    int len_output,
    bool *ptr_is_null
    );
extern int 
chk_test_name(
    const char *X
    );
extern int 
chk_variant_name(
    const char *X
    );
extern uint64_t 
timestamp(
    void
    );
extern bool 
dir_exists (
    const char *dir_name
    );
extern bool 
file_exists(
    const char *filename
    );
extern uint64_t 
RDTSC(
    void
    );
extern int
    chk_uuid(
        char *X,
        int desired_len
        );
extern bool
is_valid_url_char(
    char c
    );
extern int
strtolower(
    char *X
    );
extern bool
chk_is_sequential(
    uint32_t proportions[AB_MAX_NUM_VARIANTS],
    int nV
    );
extern 
int
num_lines(
        const char *filename,
        int *ptr_nl
        );
int
read_to_chars(
    char **ptr_line,
    const char *breakers,
    char *buf,
    int bufsz
    );
extern int 
  validate_chars(
    char *X,
    char *valid_chars
    );
extern bool
chk_tracer(
    const char * tracer
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
    char *g_err, 
    char *g_rslt
    );
extern void
  get_tracer(
      const char *args, 
      char *in_tracer
      );
extern void
  set_tracer(
      char *tracer,
      size_t n
      );

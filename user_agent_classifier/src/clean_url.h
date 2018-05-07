extern int
clean_url(
    const char *in_url, 
    char *out_url,
    size_t n_out
    );
extern int
idx_in_set(
    const char *in,
    const char **X, /* array of [nX] strings */
    size_t nX,
    int *ptr_idx
    );

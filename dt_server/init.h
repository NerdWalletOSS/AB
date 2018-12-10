extern void
zero_cfg(
    void
    );
extern void
free_globals(
    void
    );
extern void
zero_globals(
    void
    );
extern void
zero_log(
    void
    );
extern int
init_lua(
    const char *const config_file
    );
extern void
free_interp(
    DT_INTERPRETER_TYPE *interp
    );

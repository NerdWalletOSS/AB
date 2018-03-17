#include "ab_incs.h"
#include "auxil.h"
#include "load_lkp.h"
#include "ab_globals.h"

#define MAXLINE 1024
#define MAX_LEN_NAME AB_MAX_LEN_LKP_NAME
//-------------------------------------------------------
int 
lkp_name_to_id(
    LKP_REC_TYPE *lkp,
    int n_lkp,
    char *name,
    uint8_t *ptr_lkp_id

    )
{
  int status = 0;
  if ( lkp == NULL ) { go_BYE(-1); }
  if ( n_lkp == 0 ) { go_BYE(-1); }
  uint8_t lkp_id = 0;
  for ( int j = 0; j < n_lkp; j++ ) {
    if ( strcasecmp(lkp[j].name, name) == 0 ) { 
      if ( ( lkp[j].id == 0 ) || ( lkp[j].id > 255 ) ) { go_BYE(-1); }
      lkp_id = lkp[j].id; break;
    }
  }
  if ( lkp_id == 0 ) { go_BYE(-1); }
  *ptr_lkp_id = lkp_id;
BYE:
  return status;
}
//-------------------------------------------------------
int
load_lkp(
    const char *lkp_file,
    LKP_REC_TYPE **ptr_lkp,
    int *ptr_n_lkp
    )
{
  int status = 0;
  FILE *dfp = NULL; 
  LKP_REC_TYPE *lkp = NULL; int n_lkp = 0;

  dfp = fopen(lkp_file, "r");
  return_if_fopen_failed(dfp, lkp_file, "r");
  status = num_lines(lkp_file, &n_lkp); cBYE(status);
  if ( n_lkp == 0 ) { go_BYE(-1); }
  lkp = malloc(n_lkp * sizeof(LKP_REC_TYPE));
  return_if_malloc_failed(lkp);
  /* read lkp */
  for ( int i = 0; ; i++ ) {
    char line[MAXLINE+1];
    if ( feof(dfp) ) { break; }
    memset(line, '\0', MAXLINE+1);
    char *cptr = fgets(line, MAXLINE, dfp); 
    if ( cptr == NULL ) {  break; }
    if ( i >= n_lkp ) { go_BYE(-1); }
    status = check_remove_eoln(line); cBYE(status);
    char *name = strtok(line,",");
    if ( name == NULL ) { go_BYE(-1); }
    char *endptr;
    char *str_idx = strtok(NULL,",");
    if ( str_idx == NULL ) { go_BYE(-1); }
    int lkp_id = 0;
    lkp_id = strtoll(str_idx, &endptr, 10);
    if ( lkp_id <= 0 ) { go_BYE(-1); }
    memset(lkp[i].name, '\0', MAX_LEN_NAME+1);
    if ( strlen(name)> MAX_LEN_NAME ) { go_BYE(-1); }
    strcpy(lkp[i].name, name);
    lkp[i].id = lkp_id;
  }
  // id must be 1, 2, 3, ...
  for ( unsigned int i = 0; i < (unsigned int)n_lkp; i++ ) { 
    bool found = true;
    if ( lkp[i].id == (i+1) ) { found = true; break; } 
    if ( !found ) { go_BYE(-1); }
  }
  *ptr_lkp   = lkp;
  *ptr_n_lkp = n_lkp;
BYE:
  fclose_if_non_null(dfp);
  return status;
}
char *
lkp_id_to_name(
    LKP_REC_TYPE *lkp,
    int n_lkp,
    uint32_t id
    )
{
  
  if ( lkp == NULL ) {  WHEREAMI; return NULL; }
  if ( n_lkp == 0 ) {  WHEREAMI; return NULL; }
  if ( id > 255 ) {  WHEREAMI; return NULL; }
  for ( int j = 0; j < n_lkp; j++ ) {
    if ( lkp[j].id == id )  {
      return lkp[j].name; 
    }
  }
  return NULL;
}
//-------------------------------------------------------

#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_test_info.h"
#include "get_test_idx.h"

#define mcr_chk_buflen(buf, Xlen, nX)  {\
  buflen = strlen(buf);  \
  if ( buflen + Xlen >= nX ) {  \
    go_BYE(-1);  \
  }  \
  else {  \
    Xlen += buflen;  \
  } \
}

int
l_test_info(
    const char *args
    )
{
  int status = 0;

  int bufsz = 7; char buf[bufsz+1]; 
  status = extract_name_value(args, "Source=", '&', buf, bufsz);
  cBYE(status);


  if ( ( *buf == '\0' ) || ( strcmp(buf, "Lua") == 0 ) ) {
    if ( g_L == NULL ) { go_BYE(-1); }
    lua_getglobal(g_L, "get_test_info");
    if ( !lua_isfunction(g_L, -1)) {
      fprintf(stderr, "Function add does not exist in lua's global space\n");
      lua_pop(g_L, 1);
      go_BYE(-1);
    }
    lua_pushstring(g_L, args); // not pushing string as it causes a copy
    status = lua_pcall(g_L, 1, 1, 0);
    if (status != 0) {
      WHEREAMI;
      fprintf(stderr, "rcalling function get_test_info failed: %s\n", lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
    }
    char* test_info = (char *)lua_tostring(g_L, 1);
    if (AB_MAX_LEN_RESULT < strlen(test_info)) {
      sprintf(g_err, "{\"error\": Length of test info is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
          AB_MAX_LEN_RESULT, strlen(test_info));
      lua_pop(g_L, 1);
      go_BYE(-1);
    } else {
      memcpy(g_buf, test_info, strlen(test_info));
      lua_pop(g_L, 1);
    }
  }
  else if ( ( *buf != '\0' ) && ( strcmp(buf, "C") == 0 ) ) {
    int test_type, test_idx;
    char test_name[AB_MAX_LEN_TEST_NAME+1];
    status = get_test_type(args, &test_type); cBYE(status);
    status = get_test_name(args,  test_name);  cBYE(status);
    status = get_test_idx(test_name, test_type, &test_idx); cBYE(status);
    if ( test_idx < 0 ) { go_BYE(-1); }
    status = c_test_info(test_idx, g_rslt, AB_MAX_LEN_RESULT); cBYE(status);
  }
  else {
    go_BYE(-1);
  }

BYE:
  return status;
}
int
c_test_info(
    int test_idx, 
    char *X,
    size_t nX
    )
{
  int status = 0;
  char buf[8192];
  char buf2[32];
  size_t Xlen, buflen;

  if ( ( X == NULL ) || ( nX == 0 ) ) { go_BYE(-1); } 
  if ( test_idx >= AB_MAX_NUM_TESTS) { go_BYE(-1); }

  TEST_META_TYPE *T = &(g_tests[test_idx]);
  strcpy(X, "{ \n");
  Xlen = strlen(X);

  sprintf(buf, "  \"name\" : \"%s\", \n", T->name);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  switch ( T->test_type ) { 
    case AB_TEST_TYPE_AB : strcpy(buf2, "ABTest"); break; 
    case AB_TEST_TYPE_XY : strcpy(buf2, "XYTest"); break; 
    default : go_BYE(-1); break; 
  }
  sprintf(buf, "  \"TestType\" : \"%s\", \n", buf2);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"InternalID\" : \"%d\", \n", test_idx);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"id\" : \"%d\", \n", T->id);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"external_id\" : \"%llu\", \n", 
      (unsigned long long)T->external_id);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  switch ( T->state ) { 
    case TEST_STATE_STARTED : strcpy(buf2, "started"); break;
    case TEST_STATE_TERMINATED : strcpy(buf2, "terminated"); break;
    default : go_BYE(-1); break;
  }
  sprintf(buf, "  \"State\" : \"%s\", \n", buf2);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"is_dev_specific\" : \"%d\", \n", T->is_dev_specific);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"seed\" : \"%" PRIu64 "\", \n", T->seed);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  if ( T->state == TEST_STATE_TERMINATED ) { 
    sprintf(buf, "  \"FinalVariantID\" : \"%u\", \n", T->final_variant_id[0]);
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    sprintf(buf, "  \"FinalVariantIDX\" : \"%u\", \n", T->final_variant_idx[0]);
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
  }

  sprintf(buf, "  \"NumVariants\" : \"%d\", \n", T->num_variants);
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  sprintf(buf, "  \"Variants\" : [ \n");
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

  for ( unsigned int i = 0; i < T->num_variants; i++ ) { 
    sprintf(buf, "    { \n"); 
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

    sprintf(buf, "  \"id\" : \"%d\", \n", 
        g_tests[test_idx].variants[i].id);
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

    sprintf(buf, "  \"name\" : \"%s\", \n", 
        g_tests[test_idx].variants[i].name);
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

    sprintf(buf, "  \"percentage\" : \"%d\", \n", 
        g_tests[test_idx].variants[i].percentage);
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    //------------------------------------------
    const char *url = g_tests[test_idx].variants[i].url;
    if ( url == NULL ) { 
      sprintf(buf, "  \"url\" : null, \n");
    }
    else {
      sprintf(buf, "  \"url\" : \"%s\", \n",  url);
    }
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    //------------------------------------------
    const char *cptr = g_tests[test_idx].variants[i].custom_data;
    if ( cptr == NULL ) { 
      sprintf(buf, "  \"custom_data\" : null, \n");
    }
    else {
      sprintf(buf, "  \"custom_data\" : %s, \n",  cptr);
    }
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

    sprintf(buf, "  \"JunkKey\" : \"JunkVal\" \n");
    mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    //------------------------------------------
    if ( i != (T->num_variants-1 ) ) {
      sprintf(buf, "},\n ");
      mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    }
    else {
      sprintf(buf, "}\n ");
      mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);
    }
  }

  sprintf(buf, "  ] \n");
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);


  sprintf(buf, "} \n");
  mcr_chk_buflen(buf, Xlen, nX); strcat(X, buf);

BYE:
  if ( status < 0 ) { memset(X, '\0', nX); }
  return status;
}

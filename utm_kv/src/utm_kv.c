#include "utm_kv.h"

void
print_utm_kv(
    UTM_REC_TYPE X,
    char *buf,
    size_t sz
    )
{
  memset(buf, '\0', sz);
  snprintf(buf, sz, " { \
      \"utm_med\" : \"%s\", \
      \"utm_camp\" : \"%s\", \
      \"utm_src\" : \"%s\" } ", 
  X.medium, X.source, X.campaign);
}

void
free_utm_kv(
    UTM_REC_TYPE X
    )
{
  free_if_non_null(X.medium);
  free_if_non_null(X.source);
  free_if_non_null(X.campaign);
}

int
utm_kv(
    const char *raw_instr,
    UTM_REC_TYPE *ptr_out
    )
{
  int status = 0;
#define NUM_NEEDLES 3
#define MAX_LEN_NEEDLE 15
  char needles[NUM_NEEDLES][MAX_LEN_NEEDLE] = 
  { "utm_medium=", "utm_source=", "utm_campaign=" };
  char *outbuf = NULL;
  bool valid_chars[256];
  char *instr = NULL;

  status = url_decode(raw_instr, &instr);  cBYE(status);

  memset(valid_chars, '\0', 256);
  for ( char c = '0'; c <= '9'; c++ ) { valid_chars[(uint8_t)c] = true; }
  for ( char c = 'a'; c <= 'z'; c++ ) { valid_chars[(uint8_t)c] = true; }
  for ( char c = 'A'; c <= 'Z'; c++ ) { valid_chars[(uint8_t)c] = true; }
  valid_chars[(uint8_t)'_'] = true;
  valid_chars[(uint8_t)'-'] = true;
  valid_chars[(uint8_t)'.'] = true;
  valid_chars[(uint8_t)' '] = true;

  ptr_out->medium = NULL;
  ptr_out->source = NULL;
  ptr_out->campaign = NULL;

  for ( int i = 0; i < NUM_NEEDLES; i++ ) {
    outbuf = NULL;
    char *needle = needles[i];
    char *val = strstr(instr, needle);
    if ( val == NULL ) { continue; } // skip if no match found
    // find longest string after needle, breaking on non alphanum,_,-
    char *start = NULL, *cptr = NULL;
    start = val + strlen(needle);
    for ( cptr = start; ; cptr++ ) { 
      if ( !valid_chars[(uint8_t)*cptr] ) { break; }
    }
    //-----------------------------------
    int len = cptr - start;
    // string not found or too long 
    if ( ( len <= 0 ) || ( len > UTM_MAX_LEN_PARAM ) ) { 
      continue;
    }
    outbuf = malloc(len+1); return_if_malloc_failed(outbuf);
    memset(outbuf, '\0', len+1);
    memcpy(outbuf, start,  len);
    for ( char *xptr = outbuf; *xptr != '\0'; xptr++ ) { 
      *xptr = tolower(*xptr);
    }
    if ( strcmp(needle, "utm_medium=") == 0 ) { 
      ptr_out->medium = outbuf;
    }
    else if ( strcmp(needle, "utm_source=") == 0 ) { 
      ptr_out->source = outbuf;
    }
    else if ( strcmp(needle, "utm_campaign=") == 0 ) { 
      ptr_out->campaign = outbuf;
     }
    else {
      // printf("22\n"); WHEREAMI; 
      go_BYE(-1);
    }
  }
BYE:
  free_if_non_null(instr);
  return status;
}

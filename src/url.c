#include "ab_incs.h"
#include "auxil.h"
#include "url.h"

/* Converts a hex character to its integer value */
// START FUNC DECL
char 
from_hex(
    char ch
    ) 
// STOP FUNC DECL
{
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
// START FUNC DECL
char 
to_hex(
    char code
    ) 
// STOP FUNC DECL
{
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
// START FUNC DECL
int 
url_encode(
    const char *in_str,
    char **ptr_out_str
    )
// STOP FUNC DECL
{
  int status = 0;
  char *pstr = NULL;
  char *out_str = NULL;
  char *pbuf = NULL;

  pstr = (char *)in_str; 
  int len = (strlen(in_str) * 3 + 1); 
  out_str = malloc(len);
  return_if_malloc_failed(out_str);
  pbuf = out_str;

  int chk_len = 0;
  while (*pstr) {
    if ( ( isalnum(*pstr) ) || ( *pstr == '-' ) || ( *pstr == '_' ) 
        || ( *pstr == '.' ) || ( *pstr == '~') ) {
      if ( chk_len >= len ) { go_BYE(-1); }
      *pbuf++ = *pstr;
      chk_len++;
    }
    else if ( *pstr == ' ') {
      if ( chk_len >= len ) { go_BYE(-1); }
      *pbuf++ = '+';
      chk_len++;
    }
    else {
      if ( chk_len >= (len-3) ) { go_BYE(-1); } 
      *pbuf++ = '%'; 
      *pbuf++ = to_hex(*pstr >> 4); 
      *pbuf++ = to_hex(*pstr & 15);
      chk_len += 3;
    }
    pstr++;
  }
  *pbuf = '\0';
  *ptr_out_str = out_str;
BYE:
  return status ;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
// START FUNC DECL
int 
url_decode(
    const char *in_str,
    char **ptr_out_str
    ) 
// STOP FUNC DECL
{
  int status = 0; 
  char *pstr;
  char *out_str;
  char *pbuf;
  
  if ( ( in_str == NULL ) || ( *in_str == '\0' ) )  { go_BYE(-1); }

  pstr = (char *)in_str; 
  int len = strlen(in_str) + 1;
  out_str = malloc(len);
  return_if_malloc_failed(out_str);
  memset(out_str, '\0', len);
  pbuf = out_str;

  int chk_len = 0;
  while (*pstr) {
    if ( chk_len > len ) { 
      go_BYE(-1); 
    }
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        if ( chk_len > len ) { go_BYE(-1); }
	*pbuf = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        // Note: We do not accept null characters
        if ( *pbuf == '\0' ) { go_BYE(-1); } 
        pbuf++;
        chk_len++;
	pstr += 2;
      }
    } 
    else if (*pstr == '+') { 
      if ( chk_len > len ) { go_BYE(-1); }
      *pbuf++ = ' ';
      chk_len++;
    } 
    else {
      *pbuf++ = *pstr;
      chk_len++;
    }
    pstr++;
  }
  *pbuf = '\0';
  *ptr_out_str = out_str;
BYE:
  return status ;

}

// START FUNC DECL
int
ext_url_encode(
    const char *in_str,
    char *rslt_buf,
    size_t sz_rslt_buf
    )
// STOP FUNC DECL
{
  int status = 0;
  char *out_str = NULL;

  status = url_encode(in_str, &out_str); cBYE(status);
  if ( strlen(out_str) >= sz_rslt_buf ) { go_BYE(-1); }
  strcpy(rslt_buf, out_str);
BYE:
  free_if_non_null(out_str);
  return status ;
}

// START FUNC DECL
int
ext_url_decode(
    const char *in_str,
    char *rslt_buf,
    size_t sz_rslt_buf
    )
// STOP FUNC DECL
{
  int status = 0;
  char *out_str = NULL;

  status = url_decode(in_str, &out_str); cBYE(status);
  if ( strlen(out_str) >= sz_rslt_buf ) { go_BYE(-1); }
  strcpy(rslt_buf, out_str);
BYE:
  free_if_non_null(out_str);
  return status ;
}


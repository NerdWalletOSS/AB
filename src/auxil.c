#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include <strings.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "macros.h"
#include "auxil.h"
#include "mmap.h"

// START FUNC DECL
int
stoF4(
      const char *X,
      float *ptr_valF4
      )
// STOP FUNC DECL
{
  int status = 0;
  char *endptr;
  double valF8;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  valF8 = strtod(X, &endptr);
  if ( ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { go_BYE(-1); }
  // TODO P1 WTF??? Following needs to be commented out??
  // if ( ( valF8 < FLT_MIN ) || ( valF8 > FLT_MAX ) ) { go_BYE(-1); }
  *ptr_valF4 = valF8;
 BYE:
  return status ;
}
//
// START FUNC DECL
int
stoI4(
      const char *X,
      int32_t *ptr_Y
      )
// STOP FUNC DECL
{
  int status = 0;
  char *endptr;
  long long Y;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { 
    go_BYE(-1); }
  Y = strtoll(X, &endptr, 10);
  if ( ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { 
    fprintf(stderr, "endptr = [%s]\n", endptr);
    fprintf(stderr, "X = [%s]\n", X);
    go_BYE(-1); 
  }
  if ( ( Y < INT_MIN ) || ( Y > INT_MAX ) ) { go_BYE(-1); }
  *ptr_Y = Y;
 BYE:
  return status ;
}

// START FUNC DECL
int
stoI8(
      const char *X,
      int64_t *ptr_Y
      )
// STOP FUNC DECL
{
  int status = 0;
  char *endptr;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  *ptr_Y = strtoll(X, &endptr, 0);
  if ( ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { 
    go_BYE(-1); 
  }
 BYE:
  return status ;
}


// START FUNC DECL
int
read_nth_val( /* n starts from 0 */
	     const char *in_str,
	     const char *delim,
	     int n,
	     char *out_str,
	     int len_out_str
	      )
// STOP FUNC DECL
{
  int status = 0;

  if ( len_out_str < 2 ) { go_BYE(-1); }
  if ( out_str == NULL ) { go_BYE(-1); }
  if (  delim  == NULL ) { go_BYE(-1); }
  if (  in_str == NULL ) { go_BYE(-1); }
  if ( n < 0 ) { go_BYE(-1); }

  // advance cptr until we have crossed n occurrences of delim
  char *cptr = (char *)in_str;
  int n_delim = strlen(delim);
  if ( n_delim <= 0 ) { go_BYE(-1); }

  int i = 0;
  for ( ; ( ( i < n )  && ( *cptr != '\0' ) && ( *cptr != '\n' ) ) ; ) { 
    if ( strncmp(cptr, delim, n_delim) == 0 ) { 
      cptr += n_delim;
      i++; 
    }
    else {
      cptr++;
    }
  }
  //------------------------
  if ( i < n ) { // Insufficient elements
    *out_str = '\0';// null terminate string 
  }
  else {
    int j = 0;
    for ( j = 0; ( ( *cptr != '\0' ) && ( *cptr != '\n' ) ) ; j++ ) { 
      if ( strncmp(cptr, delim, n_delim) == 0 ) { break; }
      if ( j == (len_out_str-1) ) { go_BYE(-1); } // no space
      out_str[j]= *cptr++;
    }
    out_str[j] = '\0'; // null terminate string 
  }

 BYE:
  return status ;
}
int
extract_name_value(
		   const char *in_str,
		   const char *start,
		   char delim,
                   char *val, /* [len] */
                   int maxlen
		   )
// STOP FUNC DECL
{
  int status = 0;
  char *cptr1, *cptr;
  int len;
  bool found_start;

  if ( in_str == NULL ) { go_BYE(-1); }
  if ( ( start == NULL ) || ( *start == '\0' ) ) { go_BYE(-1); }
  if ( delim  == '\0' ) { go_BYE(-1); }

  for ( found_start = false; ; ) { 
    cptr1 = strstr(in_str, start);
    // If no match found, then quit 
    if ( ( cptr1 == NULL ) || ( *cptr1 == '\0' ) ) { 
      found_start = false; break;
    }
    // If match from start of line, great. Break loop and keep moving
    if ( cptr1 == in_str ) { found_start = true; break; } 
    // See if there was a delimiter the match location
    if ( *(cptr1-1) == delim ) { found_start = true; break; }
    in_str += (cptr1 - in_str) + strlen(start);
  }
  if ( !found_start ) { goto BYE; }

  // Now advance to after the start string and look for stop string
  cptr1 += strlen(start);
  cptr = cptr1;
  for ( len = 0; ; len++, cptr1++ ) { 
    if ( ( *cptr1 == delim ) || ( *cptr1 == '\0' ) ) {
      break;
    }
  }
  if ( len > maxlen ) { 
    WHEREAMI;
    fprintf(stderr, "Input string = [%s], start=[%s], exceeded len = %d \n", in_str, start, maxlen);
    go_BYE(-1); 
  }
  memset(val, '\0', maxlen);
  strncpy(val, cptr, len);
  val[len] = '\0'; // null terminate string. space better exist!
BYE:
  return status ;
}
// START FUNC DECL
int
strip_trailing_slash(
		     const char *in_str, 
		     char *out_str,
		     int out_len
		     )
// STOP FUNC DECL
{
  int status = 0;
  if ( in_str == NULL ) { go_BYE(-1); }
  if ( out_str == NULL ) { go_BYE(-1); }
  if ( out_len <= 0 ) { go_BYE(-1); }

  memset(out_str, '\0', out_len);
  int in_len = strlen(in_str);
  if ( in_len >= out_len ) { go_BYE(-1); }
  strcpy(out_str, in_str);
  for ( int i = in_len-1; i >= 0 ; i-- ) { 
    if ( out_str[i] == '/' ) { 
      out_str[i] = '\0';  
    }
    else {
      break;
    }
  }
  if ( strlen(out_str) == 0 ) { go_BYE(-1); }
 BYE:
  return status ;
}
// START FUNC DECL
int
count_char(
	   const char *X,
	   char c,
	   int *ptr_n
	   )
// STOP FUNC DECL
{
  int status = 0;
  if ( X == NULL ) { go_BYE(-1); }
  int n = 0, len = strlen(X);
  
  for ( int i = 0; i < len; i++ ) { 
    if ( X[i] == c ) { n++; }
  }
  *ptr_n = n;
 BYE:
  return status ;
}

// START FUNC DECL
int
strip_extra_spaces(
		   char *X
		   )
// STOP FUNC DECL
{
  int status = 0;
  if ( X == NULL )  { go_BYE(-1); }
  int i, len;
  len = strlen(X); if ( len == 0 ) { goto BYE; }
  // strip trailing spaces
  for ( i = len-1 ; i >= 0; i++ ) { 
    if ( isspace(X[i]) ) { X[i] =  '\0'; } else { break; }
  }
  len = strlen(X); if ( len == 0 ) { goto BYE; }
  // strip leading spaces
  int k = 0;
  for ( i = 0; i < len; i++ ) { 
    if ( isspace(X[i]) ) {
      k=i+1;
    }
    else {
      break;
    }
  }
  //-----------------
  for ( i = 0; i < len-k; i++ ) {
    X[i] = X[i+k];
  }
  for ( i = len -k; i < len; i++ ) { 
    X[i]= '\0';
  }
  //-----------------
  // strip extra spaces in between
 BYE:
  return status ;
}

uint64_t 
timestamp(
    void
    )
{
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  uint64_t sec = time.tv_sec;
  uint64_t nsec = time.tv_nsec;
  return ( sec * 1000000000 )  + nsec;
}
// START FUNC DECL
uint32_t get_time_sec(
    void
    )
// STOP FUNC DECL
{
  struct timeval Tps;
  struct timezone Tpf;

  gettimeofday (&Tps, &Tpf);
  return (uint32_t )Tps.tv_sec;
}


// START FUNC DECL
uint64_t get_time_usec(
    void
    )
// STOP FUNC DECL
{
  struct timeval Tps;
  struct timezone Tpf;
  unsigned long long t = 0, t_sec = 0, t_usec = 0;

  gettimeofday (&Tps, &Tpf);
  t_sec  = (uint64_t )Tps.tv_sec;
  t_usec = (uint64_t )Tps.tv_usec;
  t = t_sec * 1000000 + t_usec;
  return t;
}

// START FUNC DECL
void
zero_string_to_nullc(
		     char *X
		     )
// STOP FUNC DECL
{
  for ( ; *X != '\0'; X++ ) { 
    *X = '\0';
  }
}
//<hdr>
unsigned int
prime_geq(
	  int n
	  )
//</hdr>
{
  for ( int m = n+1; ; m++ ) {
    if ( is_prime(m) ) {
      return m;
    }
  }
}

//<hdr>
bool
is_prime(
	 unsigned int n
	 )
//</hdr>
{
  if ( n == 0 ) { return false; }
  if ( n == 1 ) { return true; }
  if ( n == 2 ) { return true; }
  if ( n == 3 ) { return true; }
  // int sqrt_n = (int)(sqrt((double)n));
  for ( unsigned int j = 5; j < n/2; j+= 2 ) { 
    if ( ( n % j ) == 0 ) {
      return false;
    }
  }
  return true;
}
//<hdr>
int
extract_S(
    const char *X,
    const char *start_str,
    const char *stop_str,
    char *output,
    int len_output,
    bool *ptr_is_null
    )
//</hdr>
{
  int status = 0;
  int i;
  *ptr_is_null = true;
  if ( X == NULL ) { *ptr_is_null = true; return status ; }
  char *Y = strstr(X, start_str);
  if ( Y == NULL ) {  *ptr_is_null = true; return status ; }
  char *Z = Y + strlen(start_str);
  char *W = strstr(Z, stop_str);
  if ( W == NULL ) {  *ptr_is_null = true; return status ; }
  if ( ( W - Z ) >= (len_output-1) ) { go_BYE(-1); } // string too long 
  for ( i = 0; Z != W ; ) {
    output[i++] = *Z++;
  }
  output[i] = '\0';
  *ptr_is_null = false;
BYE:
  return status ;
}

//<hdr>
bool 
dir_exists (
    const char *dir_name
    )
//</hdr>
{
  DIR *dir = NULL;
  if ( ( dir_name == NULL ) || ( *dir_name == '\0' ) ) { return false; }
  dir = opendir(dir_name);
  if ( dir == NULL ) { return false; }
  free_if_non_null(dir);
  return true;
}

//<hdr>
bool 
file_exists(
    const char *filename
    )
//</hdr>
{
  int status = 0; struct stat buf;
  if ( ( filename == NULL ) || ( *filename == '\0' ) ) { return false; }
  status = stat(filename, &buf );
  if ( status == 0 ) { /* File found */
    return true;
  }
  else {
    return false;
  }
}

/* assembly code to read the TSC */
inline uint64_t 
RDTSC(void)
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

#ifdef AUXIL_STAND_ALONE
int
main()
{
  int status = 0;
  const char *in = "TestName=0123456789012345678901234567890&e=10&me=20";
  int len  = 32;
  char test_name[32];
  char var_name[15];

  memset(test_name, '\0', 32);

  status = extract_name_value(in, "TestName=", '&', test_name, len-1);
  fprintf(stderr, "TestName=[%s]\n", test_name);
  status = extract_name_value(in, "e=", '&', test_name, len-1);
  fprintf(stderr, "e=[%s]\n", test_name);
  status = extract_name_value(in, "me=", '&', test_name, len-1);
  fprintf(stderr, "me=[%s]\n", test_name);
  status = extract_name_value("SetState?TestName=ToNADTZYsNwXftPgHiYMTk5D3CTHWd&NewState=terminated&TestType=ABTest&FinalVariant=SRZ9jyWJPZQxXyx", "FinalVariant=", '&', var_name, 15);
  fprintf(stderr, "variant=[%s]\n", var_name);
BYE:
  return status;
}
#endif

bool
is_valid_url_char(
    char c
    )
{
  const char *ok_chars = "-._~:/?#[]@!$&'()*+,;=";
  if ( isalnum(c) ) { return true; }
  for ( char *cptr = (char *)ok_chars; *cptr != '\0'; cptr++ ) { 
    if ( c == *cptr ) { return true; }
  }
  return false;
}
/* URL characters must be in 
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
0123456789
-._~:/?#[]@!$&'()*+,;=
*/

int
strtolower(
    char *X
    )
{
  int status = 0;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }

  for ( char *cptr = X; *cptr != '\0'; cptr++ ) { 
    *cptr = tolower(*cptr);
  }
BYE:
  return status;
}

int
num_lines(
    const char *filename,
    int *ptr_nl
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  *ptr_nl = 0;
  status = rs_mmap(filename, &X, &nX, 0); cBYE(status);
  status = count_char(X, '\n', ptr_nl); cBYE(status);
BYE:
  rs_munmap(X, nX);
  return status;
}

int
read_to_chars(
    char **ptr_line,
    const char *breakers,
    char *buf,
    int bufsz
    )
{
  int status = 0;
  if ( ( breakers == NULL ) || ( *breakers == '\0' ) ) { go_BYE(-1); }
  if ( buf == NULL ) { go_BYE(-1); }
  memset(buf, '\0', bufsz);
  char *line = *ptr_line;
  int nb = strlen(breakers);
  bool breaker_seen = false;
  for ( int bufidx = 0; *line != '\0' ; line++ ) { 
    for ( int i = 0; i < nb; i++ ) { 
      if ( *line == breakers[i] ) { breaker_seen = true; break; }
    }
    if ( breaker_seen ) { break; }
    if ( bufidx >= bufsz ) { go_BYE(-1); }
    buf[bufidx++] = *line;
  }
  if ( *line == ',' ) { line++; } // skip over comma
  *ptr_line = line;
BYE:
  return status;
}

int 
  validate_chars(
    char *X,
    char *valid_chars
    )
{
  int status = 0;
  for ( char *cptr = X; *cptr != '\0'; cptr++ ) { 
    if ( !valid_chars[(uint8_t)(*cptr)] ) { 
      go_BYE(-1); 
    }
  }
BYE:
  return status;
}

int
check_remove_eoln(
    char *line
    )
{
  int status = 0;
  for ( char *cptr = line; *cptr != '\0'; cptr++ ) {
    if ( *cptr == '\n' ) {
      if ( *(cptr+1) != '\0' )  { go_BYE(-1); }
      *cptr = '\0';
    }
  }
BYE:
  return status;
}
//-------------------------------------------------------
int
strip_dquotes(
    char *X
    ) 
{
  int status = 0;

  if ( X[0] == '"' ) { 
    int len = strlen(X);
    if ( X[len-1] != '"' ) { go_BYE(-1); } // ending dquote 
    for ( int k = 0; k < len-1; k++ ) { 
      X[k] = X[k+1];
    }
    X[len-1] = '\0';
    X[len-2] = '\0';
  }
BYE:
  return status;
}

bool 
chk_tracer(
    const char * tracer,
    size_t max_len
    )
{
  if ( *tracer == '\0' ) { return false; }
  size_t len = 0;
  for ( char *cptr = (char *)tracer; *cptr != '\0'; cptr++, len++ ) { 
    if ( len > max_len ) { return false; }
    if ( isalnum(*cptr) || ( *cptr == '_' ) || ( *cptr == '-' ) ) {
      // all is well 
    }
    else {
      return false;
    }
  }
  return true;
}


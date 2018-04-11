//START_INCLUDES
#include <stdio.h>
#include <string.h>
#include "dt_incs.h"
//STOP_INCLUDES
#include "get_cell.h"
//START_FUNC_DECL
size_t
get_cell(
    char *X,
    size_t nX,
    size_t xidx,
    bool is_last_col,
    char *buf,
    size_t bufsz
    )
//STOP_FUNC_DECL
{
  int status = 0;
  char dquote = '"'; char comma = ','; 
  char bslash = '\\'; char eoln = '\n';
  uint32_t bufidx = 0;
  //--------------------------------
  if ( X == NULL ) { go_BYE(-1); }
  if ( nX == 0 ) { go_BYE(-1); }
  if ( xidx == nX ) { go_BYE(-1); }
  if ( buf == NULL ) { go_BYE(-1); }
  if ( bufsz == 0 ) { go_BYE(-1); }
  memset(buf, '\0', bufsz);
  char last_char;
  bool start_dquote = false;
  if ( X[xidx] == dquote ) { // must end with dquote
    start_dquote = true;
    last_char = '"';
    xidx++;
  }
  else {
    if ( is_last_col ) { 
      last_char = eoln;
    }
    else {
      last_char = comma;
    }
  }
  //----------------------------
  for ( ; ; ) { 
    if ( xidx > nX ) { go_BYE(-1); }
    if ( xidx == nX ) { return xidx; }
    if ( X[xidx] == last_char ) {
      xidx++; // jumo over last char;
      if ( start_dquote ) { 
        if ( xidx >= nX ) { go_BYE(-1); }
        if ( is_last_col ) { 
          if ( X[xidx] != eoln ) { go_BYE(-1); }
        }
        else {
          if ( X[xidx] != comma ) { go_BYE(-1); }
        }
        xidx++;
      }
      return xidx;
    }
    //---------------------------------
    if ( X[xidx] == bslash ) {
      xidx++;
      if ( xidx >= nX ) { go_BYE(-1); }
      if ( bufidx >= bufsz ) { go_BYE(-1); }
      buf[bufidx++] = X[xidx++];
      continue;
    }
    if ( bufidx >= bufsz ) { 
      go_BYE(-1); }
    buf[bufidx++] = X[xidx++];
  }
BYE:
  if ( status < 0 ) { xidx = 0; }
  return xidx;
}

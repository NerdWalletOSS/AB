#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "ua_types.h"
#include "macros.h"
#include "auxil.h"
#include "load_justin_map.h"

#define MAX_LEN_NAME 15
#define MAXLINE 127

int
load_justin_map(
    const char *justin_map_file, 
    JUSTIN_MAP_REC_TYPE **ptr_justin_map, 
    int *ptr_n_justin_map
    )
{
  int status = 0;
  FILE *dmfp = NULL;
  JUSTIN_MAP_REC_TYPE *justin_map = NULL;
  int n_justin_map = 0;

  dmfp = fopen(justin_map_file, "r");
  return_if_fopen_failed(dmfp, justin_map_file, "r");

  status = num_lines(justin_map_file, &n_justin_map); cBYE(status);
  if ( n_justin_map == 0 ) { go_BYE(-1); }
  justin_map = malloc(n_justin_map * sizeof(JUSTIN_MAP_REC_TYPE));
  return_if_malloc_failed(justin_map);
  for ( int i = 0; i < n_justin_map; i++ ) { 
    char buf[MAX_LEN_NAME+1]; int bufsz = MAX_LEN_NAME;
    char line[MAXLINE+1];
    memset(line, '\0', MAXLINE+1);
    char *cptr = fgets(line, MAXLINE, dmfp); 
    if ( cptr == NULL ) {  break; }
    strtolower(line);
    cptr = line;
    if ( line[strlen(line)-1] == '\n' ) { 
      line[strlen(line)-1] = '\0';
    }
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(justin_map[i].justin_cat, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(justin_map[i].device, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(justin_map[i].device_type, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(justin_map[i].os, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(justin_map[i].browser, buf); 
  }
  *ptr_justin_map = justin_map;
  *ptr_n_justin_map = n_justin_map;
BYE:
  return status;
}


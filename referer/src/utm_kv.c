#include "url.h"
#include "utm_incs.h"
#include "utm_types.h"
#include "utm_kv.h"

int
print_utm_kv(
    UTM_REC_TYPE X,
    char *buf,
    size_t sz
    )
{
  memset(buf, '\0', sz);
  snprintf(buf, sz, " { \
      \"medium\" : \"%s\", \
      \"campaign\" : \"%s\", \
      \"source\" : \"%s\" } ", 
  X.medium, X.source, X.campaign);
}

int
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
    const char *instr,
    UTM_REC_TYPE *ptr_out
    )
{
  int status = 0;
#define NUM_NEEDLES 3
#define MAX_LEN_NEEDLE 15
  char needles[NUM_NEEDLES][MAX_LEN_NEEDLE] = 
  { "utm_medium=", "utm_source=", "utm_campaign=" };
  char *outbuf = NULL;

  ptr_out->medium = NULL;
  ptr_out->source = NULL;
  ptr_out->campaign = NULL;

  for ( int i = 0; i < NUM_NEEDLES; i++ ) {
    char *needle = needles[i];
    char *val = strstr(instr, needle);
    char *start = NULL, *cptr = NULL;
    if ( val != NULL ) { 
      start = val + strlen(needle);
      for ( cptr = start; 
          ( ( *cptr != '\0' ) && ( *cptr != '#' ) && ( *cptr != '&' ) );
          cptr++ ) {
      }
    }
    int len = cptr - start;
    if ( ( cptr != NULL ) && 
        ( len >= 1 ) && ( len <= UTM_MAX_LEN_PARAM ) ) { 
      char inbuf[UTM_MAX_LEN_PARAM+1];
      memset(inbuf, '\0', UTM_MAX_LEN_PARAM+1);
      memcpy(inbuf, start,  len);
      status = url_decode(inbuf, &outbuf);
      if ( status < 0 ) { WHEREAMI; continue; }
      for ( char *xptr = outbuf; *xptr != '\0'; xptr++ ) { 
        *xptr = tolower(*xptr);
      }
    }
    else { 
      printf("11\n"); WHEREAMI; 
    }
    if ( outbuf != NULL ) { 
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
        printf("22\n"); WHEREAMI; 
        go_BYE(-1);
      }
    }
  }
BYE:
  return status;
}

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int sz = 1024;
  char buf[sz];
  UTM_REC_TYPE utm_rec;
  const char *instr = "https://www.nerdwallet.com/l/banking/best-cd-rates?nw_campaign_id=150845972646889300&utm_source=goog&utm_medium=cpc&utm_campaign=bk_mktg_paid_121417_beta_CDs&utm_term=%2Bcd&utm_content=ta&mktg_hline=7517&mktg_body=1699&mktg_place=5&gclid=Cj0KCQiAzMDTBRDDARIsABX4AWzrC8Y_TgM6-tfQomcrrVrI21SFPj946phFQQtsJX_8l3xiIwBPpZEaAgCPEALw_wcB&gclsrc=aw.ds&VariantID=3334&GUID=110043&DeviceID=0";
  status = utm_kv(instr, &utm_rec);
  cBYE(status);
  print_utm_kv(utm_rec, buf, sz);
  fprintf(stderr, "%s\n", buf);
  free_utm_kv(utm_rec);
BYE:
  return status;
}

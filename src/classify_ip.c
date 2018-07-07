#include "ab_incs.h"
#include "classify_ip.h"
#ifdef MAXMIND
#include "maxminddb.h"
extern MMDB_s g_mmdb; extern bool g_mmdb_in_use;
#endif

//<hdr>
int
classify_ip(
    const char *ip_addr,
    MAXMIND_REC_TYPE *ptr_maxmind
    )
//</hdr>
{
#ifdef MAXMIND
  int status = 0;

  if ( ( ip_addr == NULL ) && ( *ip_addr == '\0' ) ) { go_BYE(-1); }

  int gai_error, mmdb_error;
  MMDB_lookup_result_s result =
    MMDB_lookup_string(&g_mmdb, ip_addr, &gai_error, &mmdb_error);
  //--- START error checking
  if ( gai_error != 0 ) {
    fprintf(stderr, "\n  Error from getaddrinfo for %s - %s\n\n",
        ip_addr, gai_strerror(gai_error));
    go_BYE(-1); 
  }

  if ( MMDB_SUCCESS != mmdb_error ) {
    fprintf(stderr, "\n  Got an error from libmaxminddb: %s\n\n",
        MMDB_strerror(mmdb_error));
    go_BYE(-1);
  }
  //--- STOP error checking
  MMDB_entry_data_s entry_data;
  memset(ptr_maxmind, '\0', sizeof(MAXMIND_REC_TYPE));
  // extract postal code
  status = MMDB_get_value(&result.entry, &entry_data,
      "postal", "code", NULL);
  if ( ( status != MMDB_SUCCESS) || 
       (!entry_data.has_data) || 
       ( entry_data.type != MMDB_DATA_TYPE_UTF8_STRING) ) {
    // Nothing to do 
  }
  else {
    int len = min(entry_data.data_size, AB_MAX_LEN_POSTAL_CODE);
    strncpy(ptr_maxmind->postal_code, entry_data.utf8_string, len);
  }
  // extract time_zone
  status = MMDB_get_value(&result.entry, &entry_data,
      "location", "time_zone", NULL);
  if ( ( status != MMDB_SUCCESS) || 
       (!entry_data.has_data) || 
       ( entry_data.type != MMDB_DATA_TYPE_UTF8_STRING) ) {
    // Nothing to do 
  }
  else {
    int len = min(entry_data.data_size, AB_MAX_LEN_TIME_ZONE);
    strncpy(ptr_maxmind->time_zone, entry_data.utf8_string, len);
  }
  // extract country
  status = MMDB_get_value(&result.entry, &entry_data,
      "country", "iso_code", NULL);
  if ( ( status != MMDB_SUCCESS) || 
       (!entry_data.has_data) || 
       ( entry_data.type != MMDB_DATA_TYPE_UTF8_STRING) ) {
    // Nothing to do 
  }
  else {
    int len = min(entry_data.data_size, AB_MAX_LEN_COUNTRY);
    strncpy(ptr_maxmind->country, entry_data.utf8_string, len);
  }
  // extract state
  status = MMDB_get_value(&result.entry, &entry_data,
      "subdivision", "names", "en", NULL);
  if ( ( status != MMDB_SUCCESS) || 
       (!entry_data.has_data) || 
       ( entry_data.type != MMDB_DATA_TYPE_UTF8_STRING) ) {
    // Nothing to do 
  }
  else {
    int len = min(entry_data.data_size, AB_MAX_LEN_STATE);
    strncpy(ptr_maxmind->state, entry_data.utf8_string, len);
  }
  // extract city
  status = MMDB_get_value(&result.entry, &entry_data,
      "city", "names", "en", NULL);
  if ( ( status != MMDB_SUCCESS) || 
       (!entry_data.has_data) || 
       ( entry_data.type != MMDB_DATA_TYPE_UTF8_STRING) ) {
    // Nothing to do 
  }
  else {
    int len = min(entry_data.data_size, AB_MAX_LEN_CITY);
    strncpy(ptr_maxmind->city, entry_data.utf8_string, len);
  }
BYE:
  return status;
#endif
}

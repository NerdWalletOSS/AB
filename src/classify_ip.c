#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "classify_ip.h"
#include "load_lkp.h"

#ifdef MAXMIND
#include "maxminddb.h"
extern MMDB_s g_mmdb; extern bool g_mmdb_in_use;
#endif

#undef USE_SAMPLE
//<hdr>
int
ext_classify_ip(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  char ip_address[AB_MAX_LEN_IP_ADDRESS+1];
#ifdef USE_SAMPLE
  MMDB_entry_data_list_s *entry_data_list = NULL;
#endif

  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  memset(ip_address, '\0', AB_MAX_LEN_IP_ADDRESS+1);
  status = extract_name_value(args, "IPAddress=", '&', 
      ip_address, AB_MAX_LEN_IP_ADDRESS);
  if ( ip_address[0] == '\0' ) { go_BYE(-1); }

#ifdef MAXMIND
  int gai_error, mmdb_error;
  MMDB_lookup_result_s result =
    MMDB_lookup_string(&g_mmdb, ip_address, &gai_error, &mmdb_error);
  //--- START error checking
  if ( gai_error != 0 ) {
    fprintf(stderr, "\n  Error from getaddrinfo for %s - %s\n\n",
        ip_address, gai_strerror(gai_error));
    go_BYE(-1); 
  }

  if ( MMDB_SUCCESS != mmdb_error ) {
    fprintf(stderr, "\n  Got an error from libmaxminddb: %s\n\n",
        MMDB_strerror(mmdb_error));
    go_BYE(-1);
  }
  //--- STOP error checking
#ifdef USE_SAMPLE
  if ( !result.found_entry) { go_BYE(-1);  }
  status = MMDB_get_entry_data_list(&result.entry,
      &entry_data_list);
  if (MMDB_SUCCESS != status) { go_BYE(-1); }
  if (NULL != entry_data_list) {
      MMDB_dump_entry_data_list(stdout, entry_data_list, 2);
  }
  MMDB_free_entry_data_list(entry_data_list); entry_data_list = NULL;
#endif
  //--- STOP STUFF THAT WORKS 
  MMDB_entry_data_s entry_data;
  memset(&g_maxmind, '\0', sizeof(MAXMIND_REC_TYPE));
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
    strncpy(g_maxmind.postal_code, entry_data.utf8_string, len);
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
    strncpy(g_maxmind.time_zone, entry_data.utf8_string, len);
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
    strncpy(g_maxmind.country, entry_data.utf8_string, len);
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
    strncpy(g_maxmind.state, entry_data.utf8_string, len);
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
    strncpy(g_maxmind.city, entry_data.utf8_string, len);
  }
#endif
  snprintf(X, nX, "{ \"PostalCode\" : \"%s\", \
                    \"TimeZone\" : \"%s\", \
                    \"Country\" : \"%s\", \
                    \"State\" : \"%s\", \
                    \"City\" : \"%s\" } ",
                    g_maxmind.postal_code,
                    g_maxmind.time_zone,
                    g_maxmind.country,
                    g_maxmind.state,
                    g_maxmind.city
      );
BYE:
#ifdef USE_SAMPLE
  if ( entry_data_list != NULL ) { 
    MMDB_free_entry_data_list(entry_data_list);
  }
#endif
  return status;
}

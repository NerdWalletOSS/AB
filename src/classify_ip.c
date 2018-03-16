#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "classify_ip.h"
#include "load_lkp.h"


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
  MMDB_entry_data_list_s *entry_data_list = NULL;

  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  memset(ip_address, '\0', AB_MAX_LEN_IP_ADDRESS+1);
  status = extract_name_value(args, "IPAddress=", '&', 
      ip_address, AB_MAX_LEN_IP_ADDRESS);
  if ( ip_address[0] == '\0' ) { go_BYE(-1); }

  int gai_error, mmdb_error;
  MMDB_lookup_result_s result =
    MMDB_lookup_string(&g_mmdb, ip_address, &gai_error, &mmdb_error);

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
  if ( result.found_entry ) {
    status = MMDB_get_entry_data_list(&result.entry, &entry_data_list);
    if ( status != MMDB_SUCCESS ) { 
      fprintf( stderr, "Got an error looking up the entry data - %s\n",
          MMDB_strerror(status));
      go_BYE(-1);
    }
    if ( entry_data_list != NULL ) {
      // MMDB_dump_entry_data_list(stdout, entry_data_list, 2);
      // MMDB_entry_data_s entry_data = entry_data_list->entry_data;
      struct addrinfo *address = NULL;
      int err2;

      MMDB_lookup_result_s r2 = MMDB_lookup_sockaddr(
          &g_mmdb, address->ai_addr, &err2);
      /*
      status = MMDB_get_value(&result.entry, &entry_data,
      "city", NULL);
      if ( entry_data.has_data ) {
        switch ( entry_data.type ) {
          case MMDB_DATA_TYPE_UTF8_STRING : printf("string\n"); break; 
          case MMDB_DATA_TYPE_DOUBLE : printf("double\n"); break; 
          case MMDB_DATA_TYPE_BYTES : printf("bytes\n"); break; 
          case MMDB_DATA_TYPE_UINT16 : printf("int16\n"); break; 
          case MMDB_DATA_TYPE_UINT32 : printf("int32\n"); break; 
          case MMDB_DATA_TYPE_MAP : printf("map\n"); break; 
          case MMDB_DATA_TYPE_INT32 : printf("int32\n"); break; 
          case MMDB_DATA_TYPE_UINT64 : printf("int64\n"); break; 
          case MMDB_DATA_TYPE_UINT128 : printf("int128\n"); break; 
          case MMDB_DATA_TYPE_ARRAY : printf("array\n"); break; 
          case MMDB_DATA_TYPE_BOOLEAN : printf("bool\n"); break; 
          case MMDB_DATA_TYPE_FLOAT : printf("float\n"); break; 
          default : go_BYE(-1); break; 
        }
      }
      */
      if ( status != MMDB_SUCCESS ) { go_BYE(-1); }
    }
  } 
  else {
    fprintf(stderr, "\n  No entry for this IP address (%s) was found\n\n",
        ip_address);
    strncpy(X, "{ \"status\":\"error\", \"message\":\"no entry found\"} \n", nX); 
    // TODO P0
  }
BYE:
  if ( entry_data_list != NULL ) { 
    MMDB_free_entry_data_list(entry_data_list);
  }
  return status;
}

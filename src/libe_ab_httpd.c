#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "ab_constants.h"
#include "ab_types.h"
#include "macros.h"
#include "auxil.h"
#define __AB_MAIN_PROGRAM
#include "halt_server.h"
#include "ab_globals.h"
#include "zero_globals.h"
#include "hard_code_config.h"
#include "get_and_classify_ua.h"
#include "post_from_log_q.h"
#include "update_config.h"
#include "init.h"
#include "ab_process_req.h"
#include "get_req_type.h"
#include "get_body.h"
#include "extract_api_args.h"
#include "get_hdr_val.h"
#include "ab_auxil.h"
#include "make_guid.h"
#include "dump_log.h"
#include "l_load_config.h"
#include "l_update_config.h"
#include "l_hard_code_config.h"
#include "setup.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <err.h>
// #include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

// These two lines should be in globals but there is this 
// unnamed struct in maxmind that throws off a gcc warning
#include "maxminddb.h"
#include "dt_types.h"
MMDB_s g_mmdb; bool g_mmdb_in_use;

extern void 
generic_handler(
    struct evhttp_request *req, 
    void *arg
    );

void 
generic_handler(
    struct evhttp_request *req, 
    void *arg
    )
{
  int status = 0;
  uint64_t t_start = RDTSC();
  struct event_base *base = (struct event_base *)arg;
  char api[AB_MAX_LEN_API_NAME+1]; 
  char args[AB_MAX_LEN_ARGS+1];
  struct evbuffer *opbuf = NULL;
  opbuf = evbuffer_new();
  if ( opbuf == NULL) { go_BYE(-1); }
  const char *uri = evhttp_request_uri(req);
  memset(g_out_tracer, '\0', AB_MAX_LEN_TRACER+1);
  memset(g_in_tracer, '\0', AB_MAX_LEN_TRACER+1);

  //--------------------------------------
  status = extract_api_args(uri, api, AB_MAX_LEN_API_NAME, 
      args, AB_MAX_LEN_ARGS);
  // START: NW Specific
  if ( strcmp(api, "api/v1/health_check") == 0 ) { 
    strcpy(g_rslt, "{ \"HealthCheck\" : \"OK\" }"); goto BYE;
  }
  // STOP:  NW Specific 
  AB_REQ_TYPE req_type = get_req_type(api); 
  if ( req_type == Undefined ) { go_BYE(-1); }
  status = get_body(req_type, req, g_body, AB_MAX_LEN_BODY, &g_sz_body); 
  cBYE(status);
  if ( req_type == Router ) {  
    status = get_and_classify_ua(req, &g_device_type_id, &g_os_id, 
        &g_browser_id, &g_justin_cat_id);
    cBYE(status);
  }
#ifdef AB_AS_KAFKA
  status = get_hdr_val(req, "XXXXX", g_ip_address, AB_MAX_LEN_IP_ADDRESS); 
  cBYE(status);
#endif
  if ( ( req_type == Router ) || 
       ( req_type == ToKafka ) || 
       ( req_type == GetVariant ) || 
       ( req_type == GetVariants ) ) {
    // status = get_date(req, g_date, AB_MAX_LEN_DATE); cBYE(status);
    status = make_guid(NULL, g_out_tracer, AB_MAX_LEN_TRACER); cBYE(status);
  }
  status = ab_process_req(req_type, api, args, g_body); cBYE(status);
  //--------------------------------------

  if ( strcmp(api, "Halt") == 0 ) {
    // TODO: P4 Need to get loopbreak to wait for these 3 statements
    // evbuffer_add_printf(opbuf, "%s\n", g_rslt);
    // evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
    // evbuffer_free(opbuf);
    free_globals();
    event_base_loopbreak(base);
  }
BYE:
  evhttp_add_header(evhttp_request_get_output_headers(req), 
      "Content-Type", "application/json; charset=UTF-8");
  if ( status < 0 ) { 
    if ( ( strcmp(api, "GetVariant") == 0 ) ||
        ( strcmp(api, "GetVariants") == 0 ) ) { 
      // These are the only 2 external APIs and hence should not 
      // get any details of our internal code structure
      evbuffer_add_printf(opbuf, "{ \"ERROR\" : \"GetVariant(s)\"");
    }
    else {
      status = mk_json_output(api, args, g_err, g_rslt);
      if ( status < 0 ) { WHEREAMI; }
      evbuffer_add_printf(opbuf, "%s", g_rslt);
    }
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", opbuf);
  }
  else  {
    if ( strcmp(api, "Router") == 0 ) { 
      evhttp_add_header(evhttp_request_get_output_headers(req), 
          "Location", g_redirect_url);
      evbuffer_add_printf(opbuf, "Redirecting .....\n"); 
      evhttp_send_reply(req, HTTP_MOVETEMP, "OK", opbuf);
    }
    else {
      evbuffer_add_printf(opbuf, "%s", g_rslt);
      evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
    }
  }
  evbuffer_free(opbuf);
  //--- Log time seen by clients
  if ( ( req_type == Router ) ||  ( req_type == GetVariant )  || 
       ( req_type == GetVariants ) || ( req_type == ToKafka ) ) {
    uint64_t t_stop = RDTSC();
    if ( t_stop > t_start ) { 
      uint64_t t_delta = t_stop - t_start;
      if ( g_statsd_link != NULL ) { 
        char *key1 = (char *)"nw.metrics.ab.get_variant_time";
        statsd_timing(g_statsd_link, key1, t_delta);
        char *key2 = (char *)"nw.metrics.ab.get_variant_ctr";
        statsd_inc(g_statsd_link, key2, 1);
      }
      g_log_response_time += t_delta;
    }
  }
  //--------------------
}

char g_config_file[AB_MAX_LEN_FILE_NAME+1];
int 
main(
    int argc, 
    char **argv
    )
{
  signal(SIGINT, halt_server);
  int status = 0;
  struct evhttp *httpd;
  struct event_base *base;
  //--------------------------------------------
  g_disable_lua = false; // NORMALLY FALSE. Just for testing
  if ( g_disable_lua ) { 
    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "WARNING!! WARNING!! WARNING!! WARNING!! \n");
    fprintf(stderr, "LUA DISABLED LUA DISABLED LUA DISABLED \n");
    fprintf(stderr, "\n\n\n");
  }
  memset(g_config_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  if ( argc != 2 ) { 
    if ( strlen(argv[2]) > AB_MAX_LEN_FILE_NAME ) { go_BYE(-1); }
    strcpy(g_config_file, argv[1]); 
  }
  status = setup(false); cBYE(status);
  if ( g_cfg.sz_log_q > 0 ) { 
    pthread_mutex_init(&g_mutex, NULL);	
    pthread_cond_init(&g_condc, NULL);
    pthread_cond_init(&g_condp, NULL);
    status = pthread_create(&g_con, NULL, &post_from_log_q, NULL);
    if ( status != 0 ) { go_BYE(-1); }
  }
  //--------------------------------------------
  // TODO if ( g_reload_on_startup ) { status = reload(); cBYE(status); }
  //--------------------------------------------
  base = event_base_new();
  httpd = evhttp_new(base);
  evhttp_set_max_headers_size(httpd, AB_MAX_HEADERS_SIZE);
  evhttp_set_max_body_size(httpd, AB_MAX_LEN_BODY);
  status = evhttp_bind_socket(httpd, "0.0.0.0", g_cfg.port); 
  if ( status < 0 ) { 
    fprintf(stderr, "Port %d busy \n", g_cfg.port); go_BYE(-1);
  }
  evhttp_set_gencb(httpd, generic_handler, base);
  event_base_dispatch(base);    
  evhttp_free(httpd);
  event_base_free(base);
BYE:
  free_globals();
  return status;
}
// 
/*

Following requested by Braad. See end of this file for details
  evhttp_add_header(evhttp_request_get_output_headers(req), 
          "Content-Type", "application/json; charset=UTF-8");

The charset change here is less important, although I would always use
UTF-8 for everything as it's the most widely supported of the modern
standards. It's the "application/json" content type that tells the
caller what format the HTTP response is in.

This isn't a problem now because most HTTP SDKs don't strictly enforce
that Content-Type is set to something specific before allowing you to
work with the response body, and the response body does pass a JSON
decode just fine, but it does mean that utilities like the JSON
pretty-printing Chrome extension I use don't pretty print the AB
service response.


*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
#include "ab_globals.h"
#include "zero_globals.h"
#include "hard_code_config.h"
#include "post_from_log_q.h"
#include "init.h"
#include "ab_process_req.h"
#include "get_ua_to_device_id.h"
#include "extract_api_args.h"
#include "dump_log.h"
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
  struct event_base *base = (struct event_base *)arg;
  char api[AB_MAX_LEN_API_NAME+1]; 
  char args[AB_MAX_LEN_ARGS+1];
  struct evbuffer *opbuf = NULL;
  opbuf = evbuffer_new();
  if ( opbuf == NULL) { go_BYE(-1); }
  const char *uri = evhttp_request_uri(req);

  //--------------------------------------
  status = extract_api_args(uri, api, AB_MAX_LEN_API_NAME, 
      args, AB_MAX_LEN_ARGS);
  status = get_ua_to_device_id(req, &g_device_idx); cBYE(status);
  status = ab_process_req(api, args); cBYE(status);
  //--------------------------------------

  if ( strcmp(api, "Halt") == 0 ) {
    // TODO: Need to get loopbreak to wait for these 3 statements
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
    status = mk_json_output(api, args, g_err, g_rslt);
    if ( status < 0 ) { WHEREAMI; }
    evbuffer_add_printf(opbuf, "%s", g_rslt);
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
}

int 
main(
    int argc, 
    char **argv
    )
{
  int status = 0;
  struct evhttp *httpd;
  struct event_base *base;
  //--------------------------------------------
  status = zero_globals(); cBYE(status); /* Done only on startup */
  if ( argc != 2 )  { go_BYE(-1); }
  hard_code_config(); // only for testing 
  status = init(); cBYE(status);
  //---------------------------------------------
  if ( g_sz_log_q > 0 ) { 
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
  evhttp_set_max_body_size(httpd, AB_MAX_LEN_POST);
  status = evhttp_bind_socket(httpd, "0.0.0.0", g_port); 
  if ( status < 0 ) { 
    fprintf(stderr, "Port %d busy \n", g_port); go_BYE(-1);
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

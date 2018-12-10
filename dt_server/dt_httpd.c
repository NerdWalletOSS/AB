#include "dt_constants.h"
#include "dt_incs.h"
#include "dt_globals.h"

#include "auxil.h"
#include "init.h"
#include "dt_auxil.h"
#include "dt_process_req.h"
#include "extract_api_args.h"
#include "get_body.h"
#include "get_req_type.h"
#include "setup.h"
#include "get_from_lua.h" // get_port()

// #include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

// These two lines should be in globals but there is this 
// unnamed struct in maxmind that throws off a gcc warning

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
  DT_REQ_TYPE req_type = Undefined;
  uint64_t t_start = RDTSC();
  struct event_base *base = (struct event_base *)arg;
  char api[DT_MAX_LEN_API_NAME+1]; 
  char args[DT_MAX_LEN_ARGS+1];
  struct evbuffer *opbuf = NULL;
  opbuf = evbuffer_new();
  if ( opbuf == NULL) { go_BYE(-1); }
  const char *uri = evhttp_request_uri(req);

  //--------------------------------------
  status = extract_api_args(uri, api, DT_MAX_LEN_API_NAME, 
      args, DT_MAX_LEN_ARGS);
  // START: NW Specific
  if ( strcmp(api, "api/v1/health_check") == 0 ) { 
    strcpy(g_rslt, "{ \"HealthCheck\" : \"OK\" }"); goto BYE;
  }
  // STOP:  NW Specific 
  req_type = get_req_type(api); 
  if ( req_type == Undefined ) { go_BYE(-1); }
  status = get_body(req_type, req, g_body, DT_MAX_LEN_BODY, &g_sz_body); 
  cBYE(status);
  status = dt_process_req(req_type, api, args, g_body); cBYE(status);
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
    status = mk_json_output(api, args, g_err, g_rslt);
    if ( status < 0 ) { WHEREAMI; }
    evbuffer_add_printf(opbuf, "%s", g_rslt);
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", opbuf);
  }
  else  {
    evbuffer_add_printf(opbuf, "%s", g_rslt);
    evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
  }
  evbuffer_free(opbuf);
  //--- Log time seen by clients
  if ( req_type == Classify ) { // TODO FIX 
    uint64_t t_stop = RDTSC();
    if ( t_stop > t_start ) { 
      uint64_t t_delta = t_stop - t_start;
      g_log_response_time += t_delta;
      g_log_num_classify++;
    }
  }
  //--------------------
}

int 
main(
    int argc, 
    char **argv
    )
{
  // signal(SIGINT, halt_server); TODO 
  int status = 0;
  struct evhttp *httpd;
  struct event_base *base;
  int port = 0;

  zero_globals();
  //----------------------------------
  memset(g_config_file, '\0', DT_MAX_LEN_FILE_NAME+1);
  if ( argc != 2 ) { go_BYE(-1); }
  if ( strlen(argv[1]) > DT_MAX_LEN_FILE_NAME ) { go_BYE(-1); }
  strcpy(g_config_file, argv[1]); 
  status = setup(g_config_file); cBYE(status);
  status = get_port(&port); cBYE(status);
  //----------------------------------
  base = event_base_new();
  httpd = evhttp_new(base);
  evhttp_set_max_headers_size(httpd, DT_MAX_HEADERS_SIZE);
  evhttp_set_max_body_size(httpd, DT_MAX_LEN_BODY);
  status = evhttp_bind_socket(httpd, "0.0.0.0", port); 
  if ( status < 0 ) { 
    fprintf(stderr, "Port %d busy \n", port); go_BYE(-1);
  }
  evhttp_set_gencb(httpd, generic_handler, base);
  event_base_dispatch(base);    
  evhttp_free(httpd);
  event_base_free(base);
BYE:
  free_globals();
  return status;
}

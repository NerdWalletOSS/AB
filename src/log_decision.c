#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "post_url.h"
#include "make_curl_payload.h"
#include "log_decision.h"
int
log_decision(
    PAYLOAD_TYPE lcl_payload
    )
{
  int status = 0;
  bool is_wait = false;
  if ( g_cfg.sz_log_q > 0  ) {
    if ( g_n_log_q >= g_cfg.sz_log_q ) { go_BYE(-1); }
    if ( g_n_log_q > g_cfg.sz_log_q - 2 ) {
      g_log_dropped_posts++; 
      goto BYE; 
    }

    pthread_mutex_lock(&g_mutex);	/* protect buffer */
    if ( g_n_log_q == g_cfg.sz_log_q ) { 
      // Control never comes here because of abandonment above
      fprintf(stderr, "Waiting for space \n");
      is_wait = true;
    }
    while ( g_n_log_q == g_cfg.sz_log_q ) {
      /* If there is no space in the buffer, then wait */
      // fprintf(stderr, "producer waiting\n");
      pthread_cond_wait(&g_condp, &g_mutex);
    }
    if ( is_wait ) { fprintf(stderr, "Got space \n"); }

    int eff_wr_idx = g_q_wr_idx % g_cfg.sz_log_q;
    g_log_q[eff_wr_idx] = lcl_payload;
    g_q_wr_idx++; 
    g_n_log_q++;
    pthread_cond_signal(&g_condc);	/* wake up consumer */
    pthread_mutex_unlock(&g_mutex);	/* release the buffer */
  }
  else {
    status = make_curl_payload(lcl_payload, g_curl_payload, AB_MAX_LEN_PAYLOAD);
    cBYE(status);
    status = post_url(g_ch, g_curl_payload, NULL);
    switch ( status ) {
      case 0  : /* all is well */ break;
      case -1 : go_BYE(-1); break;
      case -2 : /* Not mission critical failure */ status = 0; break; 
      default : go_BYE(-1); break;
    }
  }
BYE:
  return status;
}

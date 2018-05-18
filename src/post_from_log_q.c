#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_variant.h"
#include "post_from_log_q.h"
#include "make_curl_payload.h"
#include "kafka_add_to_queue.h"
#include "kafka_close_conn.h"

void *
post_from_log_q(
    void *arg
    )
{
  int status = 0;
  CURLcode curl_res; 
  long http_code;
  PAYLOAD_REC_TYPE lcl_payload;
  void *kafka_payload = NULL;

  for ( ; g_halt == false ; ) {
    pthread_mutex_lock(&g_mutex);	/* protect buffer */
    if ( (g_halt == true) && ( g_n_log_q == 0 ) ) {
      pthread_mutex_unlock(&g_mutex);	/* release the buffer */
      break; // get out of the loop and out of here
    }
    while ( (g_halt == false) && ( g_n_log_q == 0 ) ) {
      /* If there is nothing in the buffer then wait */
      pthread_cond_wait(&g_condc, &g_mutex);
      // fprintf(stderr, "consumer still waiting\n");
    }
    if ( (g_halt == true) && ( g_n_log_q == 0 ) ) {
      pthread_mutex_unlock(&g_mutex);	/* release the buffer */
      break; // get out of the loop and out of here
    }
    // fprintf(stderr, "consumer read %d\n", buf[ridx]);
    int eff_rd_idx = g_q_rd_idx % g_cfg.sz_log_q;
#ifdef AB_AS_KAFKA
    kafka_payload = g_log_q[eff_rd_idx];
    g_log_q[eff_rd_idx] = NULL;
#else
    lcl_payload = g_log_q[eff_rd_idx];
    memset(&(g_log_q[eff_rd_idx]), '\0', sizeof(PAYLOAD_REC_TYPE));
#endif
    g_q_rd_idx++; 
    g_n_log_q--;
    pthread_cond_signal(&g_condp);	/* wake up consumer */
    pthread_mutex_unlock(&g_mutex);	/* release the buffer */
    // Now that you are out of the critical section, do the POST
#ifdef AB_AS_KAFKA
    status = kafka_add_to_queue((char *)kafka_payload); 
    if ( status != 0 ) { WHEREAMI; }
    printf("Freed %8x \n", (unsigned int)kafka_payload);
    free_if_non_null(kafka_payload); 
    continue;
#endif
    if ( g_ch == NULL ) { /* Nothing to do */ continue; }
    // Now, here is the real work of this consumer - the POST
    status = make_curl_payload(lcl_payload, g_curl_payload, AB_MAX_LEN_PAYLOAD);
    if ( g_rk == NULL ) { // use logger 
      curl_easy_setopt(g_ch, CURLOPT_POSTFIELDS, g_curl_payload);
      g_log_posts++;
      int retry_count = 0;
      bool post_succeeded = false;
      for ( ;  retry_count < g_cfg.num_post_retries ; retry_count++ ) {
        curl_res = curl_easy_perform(g_ch);
        if ( curl_res != CURLE_OK ) { 
          g_log_failed_posts++; continue;
        }
        curl_easy_getinfo(g_ch, CURLINFO_RESPONSE_CODE, &http_code);
        if ( http_code != 200 )  { 
          g_log_failed_posts++; continue;
        }
        // If control comes here, it means we succeeded
        post_succeeded = true;
        break;
        // Should we sleep and give logger some breathing room?
      }
      if ( !post_succeeded ) { 
        g_log_bad_posts++;
        if ( g_cfg.verbose ) { fprintf(stderr, "POST totally failed\n");  }
      }
    }
    else { // use kafka
#ifdef KAFKA
      status = kafka_add_to_queue(g_curl_payload); 
      if ( status != 0 ) { WHEREAMI; }
#endif
    }
  }
#ifdef AB_AS_KAFKA

  fprintf(stderr, "Waiting for kafka to flush. \n");
  for ( ; ; ) { 
    rd_kafka_poll(g_rk, 50);
    int len = rd_kafka_outq_len(g_rk);
    if ( len == 0 ) { break; }
    fprintf(stderr, "Waiting for kafka to flush. %d to go \n", len); 
  }
#endif
  // pthread_exit(NULL); TODO P0 IS THIS THE RIGHT THING TO DO 
  return NULL;
}

#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_variant.h"
#include "post_from_log_q.h"
#include "make_curl_payload.h"
#include "kafka_add_to_queue.h"
#include "statsd.h"
// #include "kafka_close_conn.h"

void *
post_from_log_q(
    void *arg
    )
{
  int status = 0;
  CURLcode curl_res; 
  long http_code;
#ifdef AB_AS_KAFKA
  KAFKA_REC_TYPE kafka_payload;
#else
  PAYLOAD_REC_TYPE lcl_payload;
#endif

  for ( ; ; ) {
    pthread_mutex_lock(&g_mutex);	/* protect buffer */
    if ( (g_halt == true) && ( g_n_log_q == 0 ) ) {
      pthread_mutex_unlock(&g_mutex);	/* release the buffer */
      // fprintf(stderr, "CONSUMER: unlock 1 %d %d \n", g_halt, g_n_log_q); 
      break; // get out of the loop and out of here
    }
    while ( (g_halt == false) && ( g_n_log_q == 0 ) ) {
      /* If there is nothing in the buffer then wait */
      // fprintf(stderr, "CONSUMER: waiting %d %d \n", g_halt, g_n_log_q);
      pthread_cond_wait(&g_condc, &g_mutex);
      // fprintf(stderr, "consumer done waiting %d %d\n", g_halt, g_n_log_q);
    }
    if ( (g_halt == true) && ( g_n_log_q == 0 ) ) {
      pthread_mutex_unlock(&g_mutex);	/* release the buffer */
      // fprintf(stderr, "CONSUMER unlock 2 %d %d \n", g_halt, g_n_log_q); 
      break; // get out of the loop and out of here
    }
    int eff_rd_idx = g_q_rd_idx % g_cfg.sz_log_q;
    // fprintf(stderr, "CONSUMER: read %d\n", eff_rd_idx);
#ifdef AB_AS_KAFKA
    kafka_payload = g_log_q[eff_rd_idx];
    memset(&(g_log_q[eff_rd_idx]), '\0', sizeof(KAFKA_REC_TYPE));
#else
    lcl_payload = g_log_q[eff_rd_idx];
    memset(&(g_log_q[eff_rd_idx]), '\0', sizeof(PAYLOAD_REC_TYPE));
#endif
    g_q_rd_idx++; 
    g_n_log_q--;
    pthread_cond_signal(&g_condp);	/* wake up producer */
    // fprintf(stderr, "CONSUMER: Woke up producer\n");
    pthread_mutex_unlock(&g_mutex);	/* release the buffer */
    // fprintf(stderr, "CONSUMER: Released buffer \n");
    // Now that you are out of the critical section, do the POST
#ifdef AB_AS_KAFKA
    uint64_t t_start = RDTSC();
    status = kafka_add_to_queue( kafka_payload.data, kafka_payload.sz);
    uint64_t t_stop = RDTSC();
    if ( t_stop > t_start ) { 
      g_log_kafka_calls++;
      STATSD_COUNT("kafka_calls", 1);
      g_log_kafka_total_time += t_stop - t_start;
    }

    if ( status != 0 ) { WHEREAMI; } // TODO P1. add statsd logging for this
    free_if_non_null(kafka_payload.data); 
    g_kafka_memory -= kafka_payload.sz;
    rd_kafka_poll(g_rk, 50); // TODO P3 Why 50?
    continue;
#endif
    // Now, here is the real work of this consumer - the POST
    status = make_curl_payload(lcl_payload, g_curl_payload, 
        AB_MAX_LEN_PAYLOAD);
    if ( status != 0 ) {
      // TODO P2 record a POST error
      WHEREAMI; continue; 
    }
    if ( !g_disable_lua ) { // use logger 
      if ( g_ch == NULL ) { 
        // TODO P2 record a POST error
        WHEREAMI; continue; 
      }
      curl_easy_setopt(g_ch, CURLOPT_POSTFIELDS, g_curl_payload);
      g_log_posts++;
      STATSD_COUNT("posts", 1);
      int retry_count = 0;
      bool post_succeeded = false;
      for ( ;  retry_count < g_cfg.num_post_retries ; retry_count++ ) {
        curl_res = curl_easy_perform(g_ch);
        if ( curl_res != CURLE_OK ) { 
          g_log_failed_posts++; STATSD_COUNT("failed_posts", 1); continue;
        }
        curl_easy_getinfo(g_ch, CURLINFO_RESPONSE_CODE, &http_code);
        if ( http_code != 200 )  { 
          g_log_failed_posts++; STATSD_COUNT("failed_posts", 1); continue;
        }
        // If control comes here, it means we succeeded
        post_succeeded = true;
        break;
        // Should we sleep and give logger some breathing room?
      }
      if ( !post_succeeded ) { 
        g_log_bad_posts++; STATSD_COUNT("bad_posts", 1);
        if ( g_cfg.verbose ) { fprintf(stderr, "POST totally failed\n");  }
      }
    }
    if ( !g_disable_kf ) { // use kafka 
      if ( g_rk == NULL ) { 
        // TODO P2 add statsd logging
        WHEREAMI; continue;
      }
      g_log_kafka_calls++; STATSD_COUNT("kafka_calls", 1);
      status = kafka_add_to_queue(g_curl_payload, 0); 
      if ( status != 0 ) { 
        // TODO P2 add statsd logging
        WHEREAMI;  continue;
      }
    }
  }
#ifdef AB_AS_KAFKA

  fprintf(stderr, "Waiting for kafka to flush. \n");
  for ( ; g_rk != NULL; ) { 
    rd_kafka_poll(g_rk, 50); // TODO P3 Why 50?
    int len = rd_kafka_outq_len(g_rk);
    fprintf(stderr, "Waiting for kafka to flush. %d in queue, %d to go \n", g_n_log_q, len); 
    if ( len == 0 ) { break; }
  }
#endif
  pthread_exit(NULL); // TODO P0 IS THIS THE RIGHT THING TO DO 
  return NULL;
}

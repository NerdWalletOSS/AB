#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "halt_server.h"
#include "zero_globals.h"
#ifdef KAFKA
#include "kafka_close_conn.h"
#endif

void halt_server(
    int sig
    )
{
  g_halt = true;
  if ( g_cfg.sz_log_q > 0 ) {
    // Tell consumer ead nothing more is coming
    pthread_cond_signal(&g_condc);  /* wake up consumer */
    fprintf(stderr, "Waiting for consumer to finish \n");
    pthread_join(g_con, NULL);
    fprintf(stderr, "Consumer finished \n");
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_condc);
    pthread_cond_destroy(&g_condp);
  }
  free_globals();
#ifdef KAFKA
  kafka_close_conn();
#endif
  exit(0);
}


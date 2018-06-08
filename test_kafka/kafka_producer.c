#include "kafka_producer.h"
EXTERN rd_kafka_t *g_rk;         /* Producer instance handle */
EXTERN rd_kafka_topic_t *g_rkt;  /* Topic object */
EXTERN rd_kafka_conf_t *g_kafka_conf;  /* Temporary configuration object */

unsigned long long get_time(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return  (unsigned long long)(tv.tv_sec) * 1000 +
    (unsigned long long)(tv.tv_usec) / 1000;

}
int main(int argc, char *argv[]){
  printf("%d\n", argc);
  if (argc != 2) {
    printf("Please have only 1 arg which is th number of times (unsigned 32 bit int)\n");
    exit(1);
  }
  int total = atoi(argv[1]);
  // scanf("%d", &total);
  KAFKA_CFG_TYPE cfg;
  strcpy(cfg.brokers, "127.0.0.1");
  strcpy(cfg.topic, "ab");
  strcpy(cfg.retries, "5");
  kafka_open_conn(cfg);
  char arr[255] = "hello";

 unsigned long long start_time, stop_time;
  
 start_time = get_time();
  for(int i=0; i < total; i++){
    kafka_add_to_queue(arr, strlen(arr));
  }
  kafka_close_conn();
  stop_time = get_time();
  printf("Time taken: %llu milliseconds\n", stop_time - start_time);
}

#include "kafka_producer.h"
EXTERN rd_kafka_t *g_rk;         /* Producer instance handle */
EXTERN rd_kafka_topic_t *g_rkt;  /* Topic object */
EXTERN rd_kafka_conf_t *g_kafka_conf;  /* Temporary configuration object */

int rand_str(char *str, size_t size)
{
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      int key = rand() % (int) (sizeof charset - 1);
      str[n] = charset[key];
    }
    str[size] = '\0';
  }
  return str;
}

unsigned long long get_time(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return  (unsigned long long)(tv.tv_sec) * 1000 +
    (unsigned long long)(tv.tv_usec) / 1000;

}
int main(int argc, char *argv[]){
  printf("%d\n", argc);
  if (argc != 3) {
    printf("Please have 2 args. The first is the number of times, and the second is the length or the random string\n");
    exit(1);
  }
  int total = atoi(argv[1]);
  size_t str_len = atoi(argv[2]);
  char * ptr = malloc(sizeof(char) * str_len);
  rand_str(ptr, str_len);
  // scanf("%d", &total);
  KAFKA_CFG_TYPE cfg;
  strcpy(cfg.brokers, "127.0.0.1");
  strcpy(cfg.topic, "ab");
  strcpy(cfg.retries, "5");
  kafka_open_conn(cfg);

  unsigned long long start_time, stop_time;

  start_time = get_time();
  for(int i=0; i < total; i++){
    kafka_add_to_queue(ptr, str_len);
  }
  kafka_close_conn();
  stop_time = get_time();
  printf("Time taken: %llu milliseconds\n", stop_time - start_time);
}

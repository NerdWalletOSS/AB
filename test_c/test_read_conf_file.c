/* 
gcc -g -Wall -std=gnu99 \
  -I../src/ -I../jansson-2.10/src/ \
  ../src/read_conf_file.c ../src/auxil.c \
  ../src/mmap.c  test_read_conf_file.c  validate_config.c  \
  ../jansson-2.10/src/.libs/libjansson.so \
  -o test_conf
*/
#include "ab_incs.h"
#include "macros.h"
#include "read_conf_file.h"
#include "validate_config.h"

bool g_disable_dt; // set to false if no decision tree
bool g_disable_ua; // set to false if no user agent classifier
bool g_disable_ip; // set to false if no MaxMind Database
bool g_disable_sd; // set to false if no statsd server
bool g_disable_wa; // disable WebApp
bool g_disable_lg; // disable Logger
bool g_disable_kf; // disable Kafka

CFG_TYPE cfg;
char g_valid_chars_in_url[256]; // Set by C
int
main(
    int argc,
    char **argv
    )
{
  int status= 0;
  const char *url_str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=/:_-%,;[].?+() ";
  for ( char *cptr = (char *)url_str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_url[(uint8_t)(*cptr)] = true;
  }
  if ( argc != 2 ) { go_BYE(-1); }
  const char *conf_file = argv[1];

  status = read_conf_file(conf_file, &cfg); cBYE(status);
  status = validate_config(cfg); cBYE(status);
  fprintf(stderr, "SUCCESS\n");
BYE:
  return status;
}

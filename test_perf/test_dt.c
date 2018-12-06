/* 
gcc -O4 -std=gnu99 test_dt.c -I../src/ ../src/mmap.c -lcurl -o test_dt
./test_dt ../DT/spam/sample_input.json 
*/
#include <stdio.h>
#include <curl/curl.h>
#include "ab_incs.h"
#include "mmap.h"
static uint64_t RDTSC( void)
{
  unsigned int lo, hi;
  asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

static size_t write_data(
    void *buffer, 
    size_t size, 
    size_t nmemb, 
    void *userp
    )
{
  // basically do nothing with response
  return size * nmemb;
}
 
int main(
    int argc,
    char **argv
    )
{
  int status = 0;
  CURL *curl = NULL;
  char *buf = NULL;
  char *X = NULL; size_t nX = 0;
  CURLcode res;
  if ( argc != 2 ) { go_BYE(-1); }
  char *post_file_name = argv[1];
  status = rs_mmap(post_file_name, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  if ( X  == NULL ) { go_BYE(-1); }
  buf = malloc(nX+1);
  memset(buf, '\0', nX+1);
  memcpy(buf, X, nX);


  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */ 
  curl = curl_easy_init();
  if ( curl == NULL ) { go_BYE(-1); }
  /* First set the URL that is about to receive our POST. This URL can
     just as well be a https:// URL if that is what should receive the
     data. */ 
  curl_easy_setopt(curl, CURLOPT_URL, "localhost:8000/Classify");
  /* Now specify the POST data */ 
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
  // where to write response
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);


  double ghz = 2500;
  /* Perform the request, res will get the return code */ 
  uint64_t t_total = 0, n_total = 0;
  double time_per; 
  for ( int i = 0; i < 100000; i++ ) { 
    uint64_t t_start = RDTSC();
    res = curl_easy_perform(curl);
    t_total += (RDTSC() - t_start);
    n_total++;
    time_per = (double)t_total/(ghz * (double)n_total);
    /* Check for errors */ 
    if ( res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));
    }
    if ( ( i % 1000 ) == 0 ) {
      fprintf(stdout, "Trials = %" PRIu64 ", time per = %lf \n",
          n_total, time_per);
    }
  }
  fprintf(stdout, "Trials = %" PRIu64 ", time per = %lf \n",
      n_total, time_per);

BYE:
  /* always cleanup */ 
  if ( curl != NULL ) { 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  free_if_non_null(buf);
  return status;
}

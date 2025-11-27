#include <curl/curl.h>
#include <stdio.h>
#include "thrd_pl.h"
#include <omp.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#define MAX_TASKS 100
#define THREADS_NB omp_get_max_threads()

/* void *display(void *name) { */
/*   unsigned int seed = time(NULL); */
/*   usleep(100000); */
/*   name =(char*) "Hello World!"; */
/*   return NULL; */
/* } */

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
   return size * nmemb;
}
void* req_worker(void* _){
    CURL *curl = NULL;
    CURLcode res;
	char* url = "https://"

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        res = curl_easy_perform(curl);
		long http_code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		printf("%ld\r\n",http_code);
        // check res, e.g. with curl_easy_strerror(res)
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return NULL;
}



int main() {

  int __state__ = 1; // Benchmark state

  JobQueue *jq = jb_queue_crt();
  JobQueue *jq_ = jb_queue_crt();
  // CREATE A JOB
  // Job *j = jb_crt(display, "@test\n", NULL);
  // Job *j_ = jb_crt(display, "@test-2\n", NULL);

  // CREATE A POOL
  ThreadPool *tp = malloc(sizeof(ThreadPool));
  ThreadPool *tp_ = malloc(sizeof(ThreadPool));

  if (tp_ == NULL) {
    fprintf(stderr, "[ERROR] Memory alllocation failure for thread pool");
    exit(-1);
  }

  init_thrd_pl(tp_, 64, jq_);
    // printf("[BOOL] %d\n",tp_->jq->f_shutdown_);
	for (int i=0;i<1000;i++){
		/* execute(tp_, display, "hello world!\n"); */
		execute(tp_,req_worker,NULL);
	}
  thrd_pl_destroy(tp_);
  // printf("SMALL\tTIME-TAKEN\n");
  // printf("%lf\n",(double) ((clock()- start_time_small)/CLOCKS_PER_SEC) *1000)
  // ;
  return 0;
}


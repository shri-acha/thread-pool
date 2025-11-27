
#include "thrd_pl.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#define MAX_TASKS 100
#define THREADS_NB omp_get_max_threads()

void *display(void *name) {
  unsigned int seed = time(NULL);
  usleep(100000);
  name =(char*) "Hello World!";
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
		execute(tp_, display, "hello world!\n");
	}
  thrd_pl_destroy(tp_);
  // printf("SMALL\tTIME-TAKEN\n");
  // printf("%lf\n",(double) ((clock()- start_time_small)/CLOCKS_PER_SEC) *1000)
  // ;
  return 0;
}

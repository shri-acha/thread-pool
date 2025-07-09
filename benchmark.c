
#include "thrd_pl.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#define MAX_TASKS 50
#define THREADS_NB omp_get_max_threads()

void *display(void *name) {
  unsigned int seed = time(NULL);
  sleep(1);
  printf("Work complete!  %d\n", rand());
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

  init_thrd_pl(tp_, 10, jq_);

  for (int i = 0; i < MAX_TASKS; i++) {
    execute(tp_, display, "hello world!\n");
  }
  thrd_pl_destroy(tp_);
  // printf("SMALL\tTIME-TAKEN\n");
  // printf("%lf\n",(double) ((clock()- start_time_small)/CLOCKS_PER_SEC) *1000)
  // ;
  return 0;
}

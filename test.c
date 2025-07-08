#include "thrd_pl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void *display(void *name) {
  unsigned int seed = time(NULL);
  printf("Work complete!  %d\n",rand());
  sleep(1);
  return NULL;
}

int main() {
  JobQueue *jq = jb_queue_crt();

  // CREATE A POOL
  ThreadPool *tp = malloc(sizeof(ThreadPool));
  if (tp == NULL) {
    fprintf(stderr, "[ERROR] Memory alllocation failure for thread pool");
    exit(-1);
  }

  // START A THREAD POOL
  // TRY TO EXECUTE A TASK IN THREAD POOLS
  // EXECUTE() CHECKS IF THE THREAD POOL IS FULL
  // IF FULL THEN PUSHES TO THE TASK QUEUE
  // ELSE IT IS PUSHED TO THREAD POOL
  // IF THREAD BECOMES EMPTY, THEN IT DEQUEUES THE TASK AND EXECUTE()
  //
  // THREAD
  // THREAD EXECUTES THE FUNCTION
  // THREAD POOL
  // IT REMAINS NEUTRAL TILL SOME TASK SHOWS UP EITHER DIRECTLY OR FROM QUEUE

  // EXECUTE THE JOBS
  //

  // res_->task(res_->args);
  // res__->task(res__->args);

  init_thrd_pl(tp,12,jq);

  while(1){
    execute(tp, display,"hello world\n");
  }

  thrd_pl_destroy(tp);
}

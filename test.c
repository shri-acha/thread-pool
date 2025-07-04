#include "thrd_pl.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *display(void *name) {
  printf("Jello %s", (char *)name);
  sleep(1);
  return NULL;
}

int main() {
  JobQueue *jq = jb_queue_crt();
  // CREATE A JOB
  Job *j_ = jb_crt(display, "@test1\n");
  Job *j__ = jb_crt(display, "@test2\n");
  Job *j___ = jb_crt(display, "@test3\n");

  // CREATE A POOL
  ThreadPool *tp = malloc(sizeof(ThreadPool));
  if (tp == NULL) { fprintf(stderr, "[ERROR] Memory alllocation failure for thread pool"); exit(-1);
  }

  insert_job(jq, j_);
  insert_job(jq, j__);

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
  Job *res_ = pop_job(jq);
  Job *res__ = pop_job(jq);

  pthread_mutex_t mtx;
  pthread_mutex_init(&mtx,NULL);

  res_->task(res_->args);
  res__->task(res__->args);
  
  execute(tp, jq);

}

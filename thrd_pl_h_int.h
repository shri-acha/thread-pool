
#ifndef _THRD_PL_INT_H

#include <stdlib.h>
#include <stdatomic.h>

#define MAX_POOL_SIZE 64
typedef void *(*jtask_t)(void *);
typedef struct Job {
  jtask_t task;
  void *args;       
  struct Job *next; // NEXT IS TOWARDS HEAD
  struct Job *prev; // PREV IS TOWARDS TAIL
  void *ret_addr; // RETURN ADDR OF THE TASK
} Job;

typedef struct JobQueue {
  struct Job *head; // HEAD BEGIN
  struct Job *tail; // TAIL END
  pthread_mutex_t qmtx;
  pthread_cond_t qcndt;
  atomic_bool f_shutdown_;
} JobQueue;

typedef struct Thread {
  pthread_t thrd_hndlr;
} Thread;

typedef struct ThreadPool {
  Thread *pool[MAX_POOL_SIZE]; // Pool
  atomic_int cur_ocpy;                // No of threads being used in the pool
  JobQueue *jq;                // Associated Job Queue
  int no_of_threads;
} ThreadPool;

JobQueue *jb_queue_crt();
Job *jb_crt(jtask_t, void *,void *);
void *insert_job(JobQueue *, Job *);
Job *pop_job(JobQueue *);
int is_queue_empty(JobQueue *);

#endif // !THRD_PL_INT_H

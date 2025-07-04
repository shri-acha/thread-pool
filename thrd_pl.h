
//            Structures required
//
//  A job queue to hold the threads/job.
//
//  A job
//
// Helpers (have to figure it out)
// execute
#include <stdlib.h>
#define MAX_POOL_SIZE 3

typedef void *(*jtask_t)(void *);

typedef struct Job {
  jtask_t task;
  void *args;
  struct Job *next; // NEXT IS TOWARDS HEAD
  struct Job *prev; // PREV IS TOWARDS TAIL
} Job;

typedef struct JobQueue {
  struct Job *head; // HEAD BEGIN
  struct Job *tail; // TAIL END
  pthread_mutex_t jq_mtx;
} JobQueue;

typedef struct Thread {
  pthread_t thrd_hndlr;
  int is_free; // 1 if thread is free, 0 is not.
} Thread;

typedef struct ThreadPool {
  Thread *pool[MAX_POOL_SIZE]; // Pool
  int cur_ocpy;               // Current Occupancy
} ThreadPool;

// THREAD AND POOL HANDLERS
//
// WHEN THE THREAD IS FREE, QUEUE IS POPPED AND TASK IS EXECUTED
//


// TASKS and JOB HANDLERS
//
// TASKS ARE FIRST ASSIGNED IN THE QUEUE
//
//
JobQueue *jb_queue_crt();
Job *jb_crt(jtask_t, void *);
void *insert_job(JobQueue *, Job *);
Job *pop_job(JobQueue *);
int is_queue_empty(JobQueue *);

void *execute(ThreadPool *tp,JobQueue *jq);

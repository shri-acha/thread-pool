#include "thrd_pl_h_int.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *execute(ThreadPool *tp, jtask_t task, void* args) {
  if (tp->jq->f_shutdown_){ // Doesn't add jobs after shutdown flag activated
    printf("[CANT ADD JOBS] - shutdown flag");
    return NULL;
  }
  Job *j_ = jb_crt(task, args, NULL);
  insert_job(tp->jq, j_);
  return NULL;
}

void *worker_thrd(void *args) {

  ThreadPool *tp = (ThreadPool *)args;
  JobQueue *jq = tp->jq;

  while (1) {

      if (tp->jq->f_shutdown_ && is_queue_empty(tp->jq)) {
          printf("[HALT]\n");
          pthread_mutex_unlock(&tp->jq->qmtx); // Unlock the mutex before exiting, else deadlock.
          pthread_exit(NULL);
      }

    pthread_mutex_lock(&jq->qmtx);
    // //  printf("Head:%p\tTail:%p\n", jq->head, jq->tail);
    while (is_queue_empty(jq)) { // Waits for condition while the queue is empty
       // printf("Waiting for qcndt...\n");
      pthread_cond_wait(&jq->qcndt, &jq->qmtx); // Threads get stuck here
       // printf("Signal received!\tHead:%p Tail:%p\n",jq->head,jq->tail);
    }
    //  printf("Popping: %p\n", jq->head);
    Job *job = pop_job(jq);
    pthread_mutex_unlock(&jq->qmtx);

    if (job->task != NULL) { // sanity check
      pthread_mutex_lock(&tp->ocpncy_mtx);
      tp->cur_ocpy++;
      pthread_mutex_unlock(&tp->ocpncy_mtx);

      job->task(job->args);

      pthread_mutex_lock(&tp->ocpncy_mtx);
      tp->cur_ocpy--;
      pthread_mutex_unlock(&tp->ocpncy_mtx);
    }

    //  printf("Freeing memory: %p\n", job);
  }
}

void init_thrd_pl(ThreadPool *tp, int num_of_threads, JobQueue *jq) {

  if (num_of_threads > MAX_POOL_SIZE) {
    fprintf(stderr, "[ERROR]: MAX SIZE OF POOL EXCEEDED(MAXPOOLSIZE = 64)");
    exit(-1);
  }
  if (jq == NULL || tp == NULL) {
    fprintf(stderr,
            "[ERROR]: PROVIDE NON NULL VALUES OF ThreadPool AND/OR JobQueue");
    exit(-1);
  }

  for (int i = 0; i < num_of_threads; i++) {
    Thread *tbuf = malloc(sizeof(Thread));
    tp->pool[i] = tbuf;
  }

  tp->jq = jq;
  tp->no_of_threads = num_of_threads;

  pthread_mutex_init(&tp->ocpncy_mtx, NULL); // initiating the occupancy mutex

  for (int i = 0; i < tp->no_of_threads; i++) {
    pthread_create(&(tp->pool[i]->thrd_hndlr), NULL, worker_thrd, tp);
  }

}

void thrd_pl_destroy(ThreadPool *tp) {

  tp->jq->f_shutdown_ = 1;

  // Order of desctruction must be 
  //
  // SHUTDOWN SIGNAL
  // JOIN THREADS 
  // DESTROY CONDITIONS 
  // DESTROY MUTEXES
  // FREE THE CUSTOM THREADS
  // FREE THE JOBS IN QUEUE IF NON-EMPTY
  //

  for (int i = 0; i < tp->no_of_threads; i++) {
    printf("[WAITING DESTRUCTION] %p\n",tp->pool[i]);
    pthread_join((tp->pool[i]->thrd_hndlr),NULL);
  }

  pthread_cond_destroy(&tp->jq->qcndt);
  pthread_mutex_destroy(&tp->jq->qmtx);

  for (int i = 0; i < tp->no_of_threads; i++) {
    free(tp->pool[i]);
  }
  printf("[ALL DONE]\n");

  while (!is_queue_empty(tp->jq)) {
    free(pop_job(tp->jq));
  }
}

struct Job *jb_crt(jtask_t f, void *args, void *ret_addr) {

  struct Job *job = (struct Job *)malloc(sizeof(struct Job));
  if (job == NULL) {
    fprintf(stderr, "[ERROR]: Memory allocation failed!");
    exit(-1);
  }
  
  job->next = NULL;
  job->prev = NULL;
  job->task = f;
  job->args = args;
  job->ret_addr = ret_addr;

  return job;
}

JobQueue *jb_queue_crt() {

  JobQueue *jq = malloc(sizeof(JobQueue));
  jq->head = NULL;
  jq->tail = NULL;
  pthread_mutex_init(&jq->qmtx, NULL);//initiating mutex variable for queue 
  pthread_cond_init(&jq->qcndt, NULL); //initiating  condition variable for queue state
  jq->f_shutdown_ = 0;

  return jq;
}

void *insert_job(struct JobQueue *jq, struct Job *job) {

  pthread_mutex_lock(&jq->qmtx);
  printf("[QUEUED]  %p\n", job);
  if (jq->head == NULL && jq->tail == NULL) { // i.e. the queue is empty
    jq->head = job;
    jq->tail = job;
    jq->head->next = NULL;
    jq->tail->prev = NULL;
    jq->head->prev = NULL;
    jq->tail->next = NULL;
  } else {                                    // non empty queue
    job->prev = jq->tail;
    job->next = NULL;
    jq->tail->next = job;
    jq->tail = job;
  }
    pthread_cond_signal(&jq->qcndt);
    pthread_mutex_unlock(&jq->qmtx);
    return NULL;
}

Job *pop_job(JobQueue *jq) {
  // When popped, the head is popped
  Job *temp;
  if (jq->head == NULL && jq->tail == NULL) { // empty state of queue
    return NULL;
  } else if (jq->head == jq->tail) { // if only one element of queue

    temp = jq->head;
    jq->head = NULL;
    jq->tail = NULL;
    return temp;

  } else { // if non empty state of queue
    temp = jq->head;
    jq->head = jq->head->next;
    if (jq->head != NULL) {
      jq->head->prev = NULL;
    }
    return temp;
  }
}

int is_queue_empty(JobQueue *jq) {
  return jq->head == NULL || jq->tail == NULL;
}

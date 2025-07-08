#include "thrd_pl.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void *worker_thrd(void *args) {

  ThreadPool *tp = (ThreadPool *)args;
  JobQueue *jq = tp->jq;

  // //  printf("Worker thread initiated....\n");
  while (!jq->f_shutdown_) {

    pthread_mutex_lock(&jq->qmtx);
    // //  printf("Head:%p\tTail:%p\n", jq->head, jq->tail);
    while (is_queue_empty(jq)) {
      //  printf("Waiting for qcndt...\n");
      pthread_cond_wait(&jq->qcndt, &jq->qmtx); // Threads get stuck here
      //  printf("Signal received!\tHead:%p Tail:%p\n",jq->head,jq->tail);
    }
    //  printf("Popping: %p\n", jq->head);
    Job *job = pop_job(jq);
    pthread_mutex_unlock(&jq->qmtx);

    if (job->task != NULL) {
      job->task(job->args);
    }

    //  printf("Freeing memory: %p\n", job);
  }
}

void init_thrd_pl(ThreadPool *tp, JobQueue *jq) {
  for (int i = 0; i < MAX_POOL_SIZE; i++) {
    Thread *tbuf = malloc(sizeof(Thread));
    tp->pool[i] = tbuf;
  }
  tp->jq = jq;
  for (int i = 0; i < MAX_POOL_SIZE; i++) {
    pthread_create(&(tp->pool[i]->thrd_hndlr), NULL, worker_thrd, tp);
  }
}

void thrd_pl_destroy(ThreadPool *tp, JobQueue *jq) {

  jq->f_shutdown_ = 1;
  pthread_cond_destroy(&jq->qcndt);
  pthread_mutex_destroy(&jq->qmtx);

  for (int i = 0; i < MAX_POOL_SIZE; i++) {
    pthread_join((tp->pool[i]->thrd_hndlr), NULL);
  }
  for (int i = 0; i < MAX_POOL_SIZE; i++) {
    free(tp->pool[i]);
  }
}

struct Job *jb_crt(jtask_t f, void *args) {

  struct Job *job = (struct Job *)malloc(sizeof(struct Job));
  if (job == NULL) {
    fprintf(stderr, "[ERROR]: Memory allocation failed!");
    exit(-1);
  }
  job->next = NULL;
  job->prev = NULL;
  job->task = f;
  job->args = args;
  return job;
}
JobQueue *jb_queue_crt() {
  JobQueue *jq = malloc(sizeof(JobQueue));
  jq->head = NULL;
  jq->tail = NULL;
  pthread_mutex_init(&jq->qmtx, NULL);
  pthread_cond_init(&jq->qcndt, NULL);
  jq->f_shutdown_ = 0;
  return jq;
}

void *insert_job(struct JobQueue *jq, struct Job *job) {
  pthread_mutex_lock(&jq->qmtx);
  if (jq->head == NULL && jq->tail == NULL) {
    jq->head = job;
    jq->tail = job;
    jq->head->next = NULL;
    jq->tail->prev = NULL;
    jq->head->prev = NULL;
    jq->tail->next = NULL;
    pthread_cond_signal(&jq->qcndt);
    pthread_mutex_unlock(&jq->qmtx);
    return NULL;
  }else {
    job->prev = jq->tail;
    job->next = NULL;
    jq->tail->next = job;
    jq->tail = job;
    pthread_cond_signal(&jq->qcndt);
    pthread_mutex_unlock(&jq->qmtx);
    return NULL;
  }
}

Job *pop_job(JobQueue *jq) {
  // When popped, the head is popped
  Job* temp;
  if (jq->head == NULL && jq->tail == NULL ){
    return NULL;
  }else if(jq->head == jq->tail) {

    temp = jq->head;
    jq->head = NULL;
    jq->tail = NULL;
    return temp;

  }else {
    temp = jq->head;
    jq->head = jq->head->next;
    if (jq->head != NULL){
      jq->head->prev = NULL;
    }
    return temp;
  }
}

int is_queue_empty(JobQueue *jq) {
  return jq->head == NULL || jq->tail == NULL;
}

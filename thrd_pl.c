#include "thrd_pl.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *execute(ThreadPool *tp, JobQueue *jq, struct Job *job) {
  if (is_pl_full(tp)) {
    insert_job(jq, job);
    while (!is_pl_full(tp)){
      spn_thrd(tp, pop_job(jq));
      return NULL;
    }
  } else {
    spn_thrd(tp, job);
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
  return jq;
}

void *insert_job(struct JobQueue *jq, struct Job *job) {
  if (jq->head == NULL || jq->tail == NULL) {
    jq->head = job;
    jq->tail = job;
    jq->head->next = job;
    jq->head->prev = NULL;
    jq->tail->prev = job;
    jq->tail->next = NULL;
    return NULL;
  }
  job->prev = jq->tail;
  job->next = NULL;
  jq->tail->next = job;
  jq->tail = job;
  return NULL;
}

Job *pop_job(JobQueue *jq) {
  if (is_queue_empty(jq)) {
    fprintf(stderr, "[ERROR] Exiting.. Job queue empty!\n");
    exit(-1);
  }
  Job *temp = jq->head;
  if (jq->head != NULL) {
    if (jq->head == jq->tail) {
      jq->head = NULL;
      jq->tail = NULL;
      return temp;
    }
    jq->head = jq->head->next;
    if (jq->head)
      jq->head->prev = NULL;
    return temp;
  }
  return NULL;
}

int is_queue_empty(JobQueue *jq) {
  if ((jq->head == jq->tail) && (jq->head == NULL || jq->tail == NULL)) {
    return 1;
  }
  return 0;
}

// void pop_pool(JobQueue *jq){
// }
void spn_thrd(ThreadPool *tp_, Job *j) {
  Thread *thrd = malloc(sizeof(Thread));
  pthread_create(&(thrd->thrd_hndlr), NULL, j->task, j->args);
  tp_->pool[tp_->cur_ocpy] = thrd;
  tp_->cur_ocpy += 1;
  tp_->pool[tp_->cur_ocpy] = NULL;
  return;
}
int is_pl_full(ThreadPool *tp_) {
  if (tp_->cur_ocpy >= MAX_POOL_SIZE) {
    return 1;
  } else {
    return 0;
  }
}

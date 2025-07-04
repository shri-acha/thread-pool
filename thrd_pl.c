#include "thrd_pl.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>





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
  if (jq != NULL) {
    if ((jq->head == jq->tail) && (jq->head == NULL || jq->tail == NULL)) {
      return 1;
    } else {
      return 0;
    }
  }
  return 0;
}

// void pop_pool(JobQueue *jq){
// }

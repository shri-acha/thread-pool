#include "thrd_pl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *display(void *name) {
  printf("Jello %s", (char *)name);
  sleep(1);
  return NULL;
}

int main() {
  JobQueue* jq = jb_queue_crt();
  Job* j_ = jb_crt(display, "@test1\n");
  Job* j__ = jb_crt(display, "@test2\n");
  Job* j___ = jb_crt(display, "@test3\n");
  ThreadPool *tp = malloc(sizeof(ThreadPool));
  execute(tp, jq,j_);
  execute(tp, jq,j__);
  execute(tp, jq,j___);
  execute(tp, jq,j__);
  sleep(1);

}

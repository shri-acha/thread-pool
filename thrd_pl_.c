// #include <pthread.h>
// #include <stdio.h>
// #include <unistd.h>
//
// // This is thus overcomplicated
// pthread_mutex_t c_mtx;
// pthread_cond_t c_cnd;
// static int counter=0;
// static int shutdown=0;
//
// void *worker(void * arg) {
//   printf("[WORKER-THREAD-INSTANTIATED]\n");
//   while (!shutdown) {
//     pthread_mutex_lock(&c_mtx);
//     pthread_cond_wait(&c_cnd, &c_mtx);
//     counter++;
//     pthread_mutex_unlock(&c_mtx);
//     sleep(1);
//     printf("[COMPLETE]\n");
//   }
// }
//
// void main() {
//   pthread_t pool[10];
//   printf("[COUNTER]: %d\n",counter);
//   for (int i = 0; i < 10; i++) {
//     pthread_create(&pool[i], NULL, worker, NULL);
//   }
//
//   for (int i = 0; i < 10; i++) {
//     pthread_cond_signal(&c_cnd);
//   }
//
//   shutdown = 1 ;
//   for (int i = 0; i < 10; i++) {
//     pthread_join(pool[i],NULL);
//   }
// }

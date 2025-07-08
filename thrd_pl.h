#ifndef _THRD_PL_H 
#include <stdlib.h>
#include "thrd_pl_h_int.h"

/**
 * @brief Creates a new thread pool.
 * * @param num_threads The number of threads to create in the pool.
 * @return nil 
 */
void init_thrd_pl(ThreadPool *,size_t no_of_threads, JobQueue *);
/**
 * @brief Destroys the thread pool.
 * * @param thread pool pointer, job queue associated
 * @return nil
 */
void thrd_pl_destroy(ThreadPool *);

/**
 * @brief execute a task given a tp and a task.
 * * @param thread pool pointer, job queue associated
 * @return nil
 */
void *execute(ThreadPool *,Job *);

#endif // !THRD_PL_H

#ifndef TPOOL_H
#define TPOOL_H
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef void (*thread_func_t) (void *args);

typedef struct tpool tpool_t;
typedef struct pool_work{
	thread_func_t func;
	void *args;
}pool_work_t;

typedef struct thread_controllers{
	pthread_mutex_t *mutex;
	pthread_cond_t *cond;
}thread_controllers_t;

struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size, bool resize);
bool tpool_destroy(tpool_t *tp);
//bool tpool_add_work(struct pool_work *w);
bool tpool_wait(tpool_t *tp);
bool tpool_add_work(tpool_t *tp, struct pool_work *w);

#endif

#include "inc/tpool.h"
#include "inc/d_queue.h"
#include "inc/core.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct tpool{
	queue_t *thread_queue;
	queue_t *work_queue;
	pthread_mutex_t mutex;
	pthread_cond_t thread_cond;
	pthread_cond_t work_cond;
};

struct pool_work{
	thread_func_t func;
	void *args;
};

struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size){
	tpool_t *pool = (tpool_t*)malloc(sizeof(tpool_t));
	if(pool == NULL) {
		fprintf(stderr, "Cannot initialize thread pool");
		return NULL;
	}

	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->thread_cond, NULL);
	pthread_cond_init(&pool->work_cond, NULL);

	pool->thread_queue = thread_queue_init(thread_queue_size);
}

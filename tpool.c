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

//struct pool_work{
//	thread_func_t func;
//	void *args;
//};

struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size, bool resize){
	tpool_t *pool = (tpool_t*)malloc(sizeof(tpool_t));
	if(pool == NULL) {
		fprintf(stderr, "Cannot initialize thread pool");
		return NULL;
	}

	pthread_mutex_init(&pool->mutex, NULL);
	pthread_cond_init(&pool->thread_cond, NULL);
	pthread_cond_init(&pool->work_cond, NULL);

	thread_controllers_t controllers = {
		.mutex = &pool->mutex,
		.cond = &pool->work_cond,
	};

	pool->work_queue = work_queue_init(work_queue_size, resize);
	pool->thread_queue = thread_queue_init(thread_queue_size, pool->work_queue, &controllers);
	return pool;
}

bool tpool_add_work(tpool_t *tp, struct pool_work *w){
	if(!w || !tp) return false;

	pthread_mutex_lock(&(tp->mutex));

	ele_t e = {.work = w};
	bool inserted = insert(tp->work_queue, e);
	if(inserted){
		pthread_cond_signal(&(tp->work_cond));
	}

	pthread_mutex_unlock(&(tp->mutex));
	return inserted;
}

bool tpool_wait(tpool_t *tp){}

bool tpool_destroy(tpool_t *tp){
	if(!tp) return false;
	if(!free_queue(tp->work_queue)) return false;
	if(!free_queue(tp->thread_queue)) return false;
	pthread_mutex_destroy(&(tp->mutex));
	pthread_cond_destroy(&(tp->thread_cond));
	pthread_cond_destroy(&(tp->work_cond));
	free(tp);
	return true;
}

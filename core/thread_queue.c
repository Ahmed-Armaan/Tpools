#include "../inc/core.h"
#include "../inc/d_queue.h"
#include "../inc/tpool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct worker_loop_args{
	queue_t *wq;
	pthread_mutex_t *mutex;
	pthread_cond_t *cond;
};

static void *worker_loop(void *args){
	struct worker_loop_args *w_l_args = (struct worker_loop_args*)args;
	if(!w_l_args){
		fprintf(stderr, "Invalid work queue");
		return NULL;
	}

	while(1){
		pthread_mutex_lock(w_l_args->mutex);
		while (w_l_args->wq->size == 0) {
			pthread_cond_wait(w_l_args->cond, w_l_args->mutex);
		}

		ele_t e = get(w_l_args->wq);
		pthread_mutex_unlock(w_l_args->mutex);
		pool_work_t *work = e.work;
		work->func(work->args);
	}

	free(w_l_args);
}

queue_t *thread_queue_init(size_t size, queue_t *work_queue, thread_controllers_t *controllers){
	if(!work_queue || work_queue->queue_type != work){
		fprintf(stderr, "Work Queue not found");
		return NULL;
	}

	queue_t *Q = new_thread_queue(size);
	if(!Q){
		fprintf(stderr, "Cannot initialize thread queue\n");
		return NULL;
	}	

	while(!is_full(Q)){
		pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t));
		if(!thread){
			fprintf(stderr, "Failed to create a thread\n");
			free(thread);
			continue;
		}

		struct worker_loop_args *w_l_args = malloc(sizeof(*w_l_args));
		w_l_args->wq = work_queue;
		w_l_args->mutex = controllers->mutex;
		w_l_args->cond = controllers->cond;

		int s = pthread_create(thread, NULL, worker_loop, (void*)(w_l_args));
		if(s != 0){
			fprintf(stderr, "Failed to create a thread\n");
			free(thread);
			continue;
		}

		ele_t e = {.thread = thread};
		insert(Q, e);
	}

	return Q;
}

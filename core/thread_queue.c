#include "../inc/core.h"
#include "../inc/d_queue.h"
#include "../inc/tpool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *worker_loop(void *args){
	while(1){}
}

queue_t *thread_queue_init(size_t size){
	queue_t *Q = new_thread_queue(size);
	if(!Q){
		fprintf(stderr, "Cannot initialize thread queue");
		return NULL;
	}	

	while(!is_full(Q)){
		pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t));
		if(!thread){
			fprintf(stderr, "Failed to create a thread");
			free(thread);
			continue;
		}

		int s = pthread_create(thread, NULL, worker_loop, NULL);
		if(s == 0){
			fprintf(stderr, "Failed to create a thread");
			free(thread);
			continue;
		}

		ele_t e = {.thread = thread};
		insert(Q, e);
	}

	return Q;
}

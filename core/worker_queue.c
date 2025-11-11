#include "../inc/core.h"
#include "../inc/d_queue.h"
#include "../inc/tpool.h"
#include <stdio.h>
#include <stdlib.h>

queue_t *work_queue_init(size_t size, bool resize){
	queue_t *Q = new_work_queue(size, resize);
	if(!Q){
		fprintf(stderr, "Cannot initialize work queue");
		return NULL;
	}	

	return Q;
}

bool push_work(queue_t *wq, pool_work_t *work){
	ele_t e = {.work = work};
	insert(wq, e);
	return true;
}

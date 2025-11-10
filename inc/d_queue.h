#ifndef D_QUEUE_H 
#define D_QUEUE_H 
#include "tpool.h"
#include <pthread.h>

typedef struct queue queue_t;
typedef union queue_element{
	pool_work_t *work;
	pthread_t *thread;
}ele_t;

typedef enum queue_type{
	threads = 1,
	work = 2,
}q_type_t;

queue_t *new_thread_queue(size_t size);
queue_t *new_work_queue(size_t size, bool resize);
//queue_t *queue_init(size_t size, q_type_t type);
//size_t expand(queue_t *Q);
bool insert(queue_t *Q, ele_t ele);
ele_t get(queue_t *Q);
bool is_full(queue_t *Q);

#endif

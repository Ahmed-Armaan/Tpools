#ifndef CORE_H 
#define CORE_H  
#include "tpool.h"
#include "d_queue.h"

queue_t *thread_queue_init(size_t size, queue_t *work_queue, thread_controllers_t *controllers);
queue_t *work_queue_init(size_t size, bool resize);
bool push_work(queue_t *wq, pool_work_t *work);

#endif

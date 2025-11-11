# Tpools

Tpools is a lightweight, C-based thread pool library that provides dynamic queues for managing worker threads and tasks efficiently.

## Features
- Dynamic queues for threads and work items  
- Optional resizing of queues  
- Type-safe insertion and retrieval  
- Minimal and modular design  

## API

**tpool_init() - Create a thread pool.**

```C
struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size, bool resize);
```
thread_queue_size - numnber of thread to use.

work_queue_size - size of task queue.

resize - set if task queue should dynamically resize.

returns a struct of type tpool

```C
struct tpool{
	queue_t *thread_queue;
	queue_t *work_queue;
	pthread_mutex_t mutex;
	pthread_cond_t thread_cond;
	pthread_cond_t work_cond;
};
```

**tpool_add_work() - pushes work into the task queue.**

```C
bool tpool_add_work(tpool_t *tp, struct pool_work *w);
```
tpool_add_work() - pushes work into the task queue.

w - a struct of type pool_work which encapsulate the function and its arguments
```C
typedef struct pool_work{
	thread_func_t func;
	void *args;
}pool_work_t;
```
**tpool_destroy() - destroys the threadpool.**

```C
bool tpool_destroy(tpool_t *tp);
```

## Example Usage
```C
#include "inc/tpool.h"
#include <stdio.h>
#include <stdlib.h>

void print_number(void *arg) { // work function
    int *num = (int *)arg;
    printf("Processing number: %d\n", *num);
    free(num);
}

int main() {
    tpool_t *pool = tpool_init(4, 8, true); // thread pool with 4 treads and task queue of initial size 8
    if (!pool) {
        fprintf(stderr, "Failed to initialize thread pool\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        int *num = malloc(sizeof(int));
        *num = i;
        pool_work_t *work = malloc(sizeof(pool_work_t));
        work->func = print_number;
        work->args = num;
        tpool_add_work(pool, work); // work added to task queue is automatically picked up and executed by any free thread
    }

    tpool_destroy(pool); // cleanup
    return 0;
}
```

#ifndef TPOOL_H
#define TPOOL_H
#include <stdbool.h>
#include <stddef.h>

typedef struct tpool tpool_t;
typedef struct pool_work pool_work_t;

typedef void (*thread_func_t) (void *args);

struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size);
bool tpool_destroy(tpool_t *tp);
bool tpool_add_work(struct pool_work *w);
bool tpool_wait(tpool_t *tp);

#endif

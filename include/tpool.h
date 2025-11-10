#ifndef TPOOL_H
#define TPOOL_H

#include <stdbool.h>
#include <stddef.h>

typedef void (*tpool_task_fn)(void *arg);
typedef struct tpool tpool_t;

tpool_t *tpool_create(size_t num_threads);
bool tpool_add_task(tpool_t *pool, tpool_task_fn func, void *arg);
void tpool_wait(tpool_t *pool);
void tpool_destroy(tpool_t *pool);

#endif 

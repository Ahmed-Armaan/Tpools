#include "tpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct tpool_task {
    tpool_task_fn func;
    void *arg;
    struct tpool_task *next;
} tpool_task_t;

struct tpool {
    pthread_t *threads;
    size_t num_threads;

    tpool_task_t *task_head;
    tpool_task_t *task_tail;

    pthread_mutex_t lock;
    pthread_cond_t notify;

    bool shutdown;
    bool started;
};

static void *tpool_worker(void *pool_arg) {
    tpool_t *pool = (tpool_t *)pool_arg;
    while (true) {
        pthread_mutex_lock(&pool->lock);
        while (!pool->shutdown && pool->task_head == NULL) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        tpool_task_t *task = pool->task_head;
        if (task) {
            pool->task_head = task->next;
            if (pool->task_head == NULL) {
                pool->task_tail = NULL;
            }
        }
        pthread_mutex_unlock(&pool->lock);

        if (task) {
            task->func(task->arg);
            free(task);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

tpool_t *tpool_create(size_t num_threads) {
    tpool_t *pool = malloc(sizeof(tpool_t));
    if (!pool) return NULL;

    pool->num_threads = num_threads;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    pool->task_head = NULL;
    pool->task_tail = NULL;
    pool->shutdown = false;
    pool->started = 0;

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);

    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&(pool->threads[i]), NULL, tpool_worker, pool);
        //pool->started++;
    }
    return pool;
}

bool tpool_add_task(tpool_t *pool, tpool_task_fn func, void *arg) {
    if (!pool || !func) return false;
    tpool_task_t *task = malloc(sizeof(tpool_task_t));
    if (!task) return false;
    task->func = func;
    task->arg = arg;
    task->next = NULL;

    pthread_mutex_lock(&pool->lock);
    if (pool->task_tail) {
        pool->task_tail->next = task;
        pool->task_tail = task;
    } else {
        pool->task_head = task;
        pool->task_tail = task;
    }
    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
    return true;
}

void tpool_wait(tpool_t *pool) {
    if (!pool) return;
    pthread_mutex_lock(&pool->lock);
    while (pool->task_head != NULL) {
        pthread_cond_wait(&pool->notify, &pool->lock);
    }
    pthread_mutex_unlock(&pool->lock);
}

void tpool_destroy(tpool_t *pool) {
    if (!pool) return;

    pthread_mutex_lock(&pool->lock);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    for (size_t i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);

    tpool_task_t *task = pool->task_head;
    while (task) {
        tpool_task_t *next = task->next;
        free(task);
        task = next;
    }

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);

    free(pool);
}

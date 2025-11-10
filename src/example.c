#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tpool.h"
#include <pthread.h>

void my_task(void *arg) {
    int num = *((int *)arg);
    printf("Task %d is running on thread %lu\n", num, (unsigned long)pthread_self());
    sleep(1);  // simulate work
    free(arg);
}

int main(void) {
    tpool_t *pool = tpool_create(4);
    if (!pool) {
        fprintf(stderr, "Failed to create thread pool\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 8; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        tpool_add_task(pool, my_task, arg);
    }

    tpool_wait(pool);
    tpool_destroy(pool);

    return EXIT_SUCCESS;
}

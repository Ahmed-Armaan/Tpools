# Tpools

Tpools is a lightweight, C-based thread pool library that provides dynamic queues for managing worker threads and tasks efficiently.

## Features
- Dynamic queues for threads and work items  
- Optional resizing of queues  
- Type-safe insertion and retrieval  
- Minimal and modular design  

---

## API Reference

### **tpool_init() — Create a thread pool**

```c
struct tpool *tpool_init(size_t thread_queue_size, size_t work_queue_size, bool resize);
```

**Parameters:**
- `thread_queue_size` — number of threads to use  
- `work_queue_size` — size of the task queue  
- `resize` — whether the task queue should dynamically resize  

**Returns:**  
A pointer to a `tpool_t` structure on success, or `NULL` on failure.

```c
struct tpool {
	queue_t *thread_queue;
	queue_t *work_queue;
	pthread_mutex_t mutex;
	pthread_cond_t thread_cond;
	pthread_cond_t work_cond;
};
```

---

### **tpool_add_work() — Add work to the task queue**

```c
bool tpool_add_work(tpool_t *tp, struct pool_work *w);
```

**Description:**  
Pushes a work item into the thread pool’s task queue.  
The task is automatically picked up and executed by any free thread.

**Parameters:**
- `tp` — pointer to the thread pool  
- `w` — a struct of type `pool_work` encapsulating the function and its arguments  

```c
typedef struct pool_work {
	thread_func_t func;
	void *args;
} pool_work_t;
```

**Returns:**  
`true` if the work was successfully added to the queue, otherwise `false`.

---

### **tpool_destroy() — Destroy the thread pool**

```c
bool tpool_destroy(tpool_t *tp);
```

**Description:**  
Destroys the thread pool, cleans up resources, and joins all worker threads.

**Parameters:**
- `tp` — pointer to the thread pool  

**Returns:**  
`true` if destroyed successfully, otherwise `false`.

---

# Usage

## Compilation

**Clone the repo**
```bash
git clone https://github.com/Ahmed-Armaan/Tpools.git
```

**Build the library**
```bash
make
```
The `libtpool.a` file will be created.

## Using libtpool

Move `libtpool.a` to your project directory.
Include the Tpools header:

```C
#include "tpools.h"
```
Use the functions as referenced in the API section

## Compiling code using libtpool

```bash
gcc <your-file>.c -L. -ltpool -pthread -Iinc -o <output_binary>
```
---


## Example Usage

```c
#include "tpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_number(void *arg) { // work function
    int *num = (int *)arg;
    printf("Processing number: %d\n", *num);
    free(num);
}

int main() {
    tpool_t *pool = tpool_init(4, 8, true); // thread pool with 4 threads and task queue of initial size 8
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

        tpool_add_work(pool, work); // add work; free threads automatically pick up and execute
    }

    sleep(10);
    tpool_destroy(pool); // cleanup
    return 0;
}
```

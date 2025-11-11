#include "../inc/d_queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

//struct queue{
//	ele_t *arr;
//	size_t size;
//	size_t cap;
//	size_t start;
//	size_t next;
//	q_type_t queue_type;
//	bool resizable;
//};

//union queue_element{
//	pool_work_t *work;
//	pthread_t *thread;
//};

queue_t *queue_init(size_t size, q_type_t type, bool resize){
	queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
	if(queue == NULL) return NULL;

	ele_t *arr = (ele_t*)malloc(sizeof(ele_t) * size);
	if(arr == NULL) return NULL;

	queue->arr = arr;
	queue->size = 0;
	queue->cap = size;
	queue->start = 0;
	queue->next = 0;
	queue->queue_type = type;
	queue->resizable = resize;
	return queue;
}

size_t expand_queue(queue_t *Q){
	ele_t *new_arr = (ele_t*)malloc(sizeof(ele_t) * 2 * Q->cap);
	if(!new_arr) return 0;

	int idx = Q->start, i = 0;
	for(;i < Q->size; i++){
		new_arr[i] = Q->arr[idx];
		idx = (idx + 1) % Q->cap;
	}

	free(Q->arr);
	Q->arr = new_arr;
	Q->start = 0;
	Q->next = i;
	Q->cap *= 2;
	return Q->cap;
}

bool check_queue_type_mismatch(queue_t *Q, ele_t ele){
	if(Q->queue_type == threads && ele.thread == NULL){
		fprintf(stderr, "Type Mismatch: Cant write work into thread queue\n");
		return true;
	}
	if(Q->queue_type == work && ele.work == NULL){
		fprintf(stderr, "Type Mismatch: Cant write tread into work queue\n");
		return true;
	}

	return false;
}

bool insert(queue_t *Q, ele_t ele){
	if(!Q) return false;
	if(check_queue_type_mismatch(Q, ele)) return false;

	if(Q->size == Q->cap && Q->resizable){
		if(expand_queue(Q) == 0){
			fprintf(stderr, "Could not expand queue: latest insertion missed\n");
			return false;
		}
	}

	Q->arr[Q->next] = ele;
	Q->next = (++Q->next) % Q->cap;
	Q->size++;
	return true;
}

ele_t get(queue_t *Q){
	if(!Q || Q->size == 0){
		ele_t empty = {0};
		return empty;
	}

	ele_t curr_ele = Q->arr[Q->start];
	Q->start = (++Q->start) % Q->cap;
	return curr_ele;
}

bool is_full(queue_t *Q){
	return(Q->size == Q->cap);
}

bool free_queue(queue_t *Q){
	if(!Q) return false;
	free(Q->arr);
	free(Q);
	return true;
}

queue_t *new_thread_queue(size_t size){
	return queue_init(size, threads, false);
}
queue_t *new_work_queue(size_t size, bool resize){
	return queue_init(size, work, resize);
}

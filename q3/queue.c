#include "queue.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t mutex;

queue* queueCreate(unsigned capacity){
	queue* q = malloc(sizeof(queue));
	q->capacity = capacity;
	q->front = q->size = 0;
	q->rear = capacity - 1;
	q->array = (int*)malloc(q->capacity * sizeof(int));
    sem_init(&mutex, 0, 1);
	return q;
}

void queueDestroy(queue* q){
	free(q);
}

int queueIsFull(queue* q){
	if (q->size == q->capacity)
		return 1;
	else
		return 0;
}

int queueIsEmpty(queue* q){
	if  (q->size == 0)
		return 1;
	else
		return 0;
}

int queueEnqueue(queue* q, int* value){
    sem_wait(&mutex);
	if (queueIsFull(q)){
        sem_post(&mutex);
		return 0;
	}
	else{
	q->rear = (q->rear + 1)%q->capacity;
	q->array[q->rear] = *value;
	q->size = q->size + 1;
	sem_post(&mutex);
	return 1;
	}
}

int queueDequeueFront(queue* queue, int* container, int* size){
    sem_wait(&mutex);
	if (queueIsEmpty(queue)){
        sem_post(&mutex);
		return 0;
	}
	else{
	*container = queue->array[queue->front];
	*size = queue->size;
	queue->front = (queue->front + 1)%queue->capacity;
	queue->size = queue->size - 1;
	sem_post(&mutex);
	return 1;
	}
}

int queueDequeueRear(queue* queue, int* container){
    sem_wait(&mutex);
	if (queueIsEmpty(queue)){
        sem_post(&mutex);
		return 0;
	}
	else{
	*container = queue->array[queue->rear];
	queue->rear = (queue->rear - 1)%queue->capacity;
	queue->size = queue->size - 1;
	sem_post(&mutex);
	return 1;
	}
}

int* queueFront(queue* queue){
	if (queueIsEmpty(queue))
		return NULL;
	else
		return &queue->array[queue->front];
}

int* queueRear(queue* queue){
	if (queueIsEmpty(queue))
		return NULL;
	else
		return &queue->array[queue->rear];
}

void queuePrint(queue* q){
	printf("queue(^: front, *: rear): [");
	int i;
	for(i = 0; i < q->capacity; i++){
		printf("%d", q->array[i]);
		if(i == q->front) printf("^");
		if(i == q->rear) printf("*");
		i == q->capacity - 1 ? printf("] ") : printf(", ");
	}
	printf("size=%d, capacity=%d\n", q->size, q->capacity);
}


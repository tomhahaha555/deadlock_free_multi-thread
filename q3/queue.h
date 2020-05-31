#include <stdlib.h>
#include <stdio.h>

/**
 * queue introduction
 *
 * queue is an implementation of int queue. The capacity of a queue is
 * pre-determined and can't be dynamically expanded after its creation.
 * Note: this queue is none-thread safe!
 *
 * Elements of struct queue is stored in an integer array.
 * The struct queue has 4 unsigned int
 * 	front:    the index of the first element in the array,
 * 	rear:     the index of the last element in the array,
 * 	size:     the number of elements in the queue,
 * 	capacity: the maximum number of elements the queue can store.
 */

struct queue_t {
	unsigned int front, rear, size, capacity;
	int * array;
};
typedef struct queue_t queue;

/*
 * queueCreate - create a queue with given capacity
 * @capacity: the capacity of the queue to be created.
 *
 * Once capacity is set, it can't be changed.
 */
queue* queueCreate(unsigned capacity);

/*
 * queueDestroy - destroy the queue and release memory
 * @q: the queue to be destroied
 *
 */
void queueDestroy(queue* q);

/*
 * queueIsFull - check if the queue is Full
 * @q: the queue to be checked
 *
 * returns 1 if number of elements reaches its capacity,
 * returns 0 otherwise.
 */
int queueIsFull(queue* q);

/*
 * queueIsEmpty - check if the queue is empty
 * @q: the queue to be checked
 *
 * returns 1 if there's no element stored in the queue,
 * returns 0 otherwise.
 */
int queueIsEmpty(queue* q);

/*
 * queueEnqueue - append an element to the queue
 * @q: the queue to which the element to be added
 * @value: the pointer to the element
 *
 * returns 1 if succeed
 * returns 0 otherwise.
 */
int queueEnqueue(queue* q, int* value);

/*
 * queueDequeueFront - remove the first element of the queue and put the element in the container
 * @q: the queue to to be dequeued from the front end
 * @container: the container to store the removed element
 *
 * returns 1 if succeed
 * returns 0 otherwise.
 */
int queueDequeueFront(queue* q, int* container, int* size);

/*
 * queueDequeueRear - remove the last element of the queue and put the element in the container
 * @q: the queue to be dequeued from the rear end
 * @container: the container to store the removed element
 *
 * returns 1 if succeed
 * returns 0 otherwise.
 */
int queueDequeueRear(queue* q, int* container);

/*
 * queueFront - get the first element in the queue, but won't remove it from the queue
 * @q: the queue
 *
 * returns pointer to the first element if queue isn't empty
 * returns NULL otherwise.
 */
int* queueFront(queue* q);

/*
 * queueFront - get the first element in the queue, but won't remove it from the queue
 * @q: the queue
 *
 * returns pointer to the first element if queue isn't empty
 * returns NULL otherwise.
 */
int* queueRear(queue* q);

/*
 * queuePrint - print out the queue
 * @q: the queue to be printed
 *
 */
void queuePrint(queue* q);

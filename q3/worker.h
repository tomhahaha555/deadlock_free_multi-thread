#include "definitions.h"
#include "job.h"
#include "queue.h"

typedef struct resource_pack {
	int space_limit;
	int num_workers;
	sem_t *sem_space;
	sem_t *sem_worker;

	sem_t *sem_skeleton;
	sem_t *sem_engine;
	sem_t *sem_chassis;
	sem_t *sem_body;

	sem_t *sem_window;
	sem_t *sem_tire;
	sem_t *sem_battery;
	sem_t *sem_car;

    sem_t *sem_stage1;
    sem_t *sem_stage2;

    sem_t *lock_body;
    sem_t *lock_skeleton;
    sem_t *lock_engine;
    sem_t *lock_chassis;
    sem_t *lock_car;
    sem_t *lock_battery;
    sem_t *lock_tire1;
    sem_t *lock_tire2;
    sem_t *lock_tire3;
    sem_t *lock_tire4;
    sem_t *lock_window1;
    sem_t *lock_window2;
    sem_t *lock_window3;
    sem_t *lock_window4;
    sem_t *lock_window5;
    sem_t *lock_window6;
    sem_t *lock_window7;


} resource_pack;

typedef struct work_pack {
	int tid;   // worker ID
	int totalqueuesize;
	queue *jobQ; // queue for job assignment
	resource_pack *resource;
} work_pack;

void* workerThreadFuc(void *arg);

void work(int jobID, resource_pack *pack, int tid);

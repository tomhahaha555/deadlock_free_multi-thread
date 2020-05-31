#include "definitions.h"
#include "main.h"
#include <omp.h>

sem_t sem_worker;
sem_t sem_space;

sem_t sem_skeleton;
sem_t sem_engine;
sem_t sem_chassis;
sem_t sem_body;

sem_t sem_window;
sem_t sem_tire;
sem_t sem_battery;
sem_t sem_car;

sem_t sem_stage1;
sem_t sem_stage2;
//divide the production of a car into 2 stages to prevent deadlock

sem_t lock_body;
sem_t lock_skeleton;
sem_t lock_engine;
sem_t lock_chassis;
sem_t lock_car;
sem_t lock_battery;
sem_t lock_tire1;
sem_t lock_tire2;
sem_t lock_tire3;
sem_t lock_tire4;
sem_t lock_window1;
sem_t lock_window2;
sem_t lock_window3;
sem_t lock_window4;
sem_t lock_window5;
sem_t lock_window6;
sem_t lock_window7;
//use semaphore to lock each process, ensure they would enter production process one by one
//to prevent deadlock due to lack of space by avoidance of deadlock

unsigned int num_cars;
unsigned static int NUM_PARTS = 17; // number of parts for making one car
unsigned int num_spaces;
unsigned int num_workers;

int main(int argc, char** argv)
{
	  if (argc < 4) {
	  printf("Usage: %s <number of cars> <number of spaces> <number of workers>\n",
	  argv[0]);
	  return EXIT_SUCCESS;
	  }
	  num_cars     = atoi(argv[1]);
	  num_spaces   = atoi(argv[2]);
	  num_workers  = atoi(argv[3]);

	///////////////////////////////////////////////////////
	printf("Name: Leung Lap Yung  UID: 3035470967\n");
	printf("Job defined, %d workers will build %d cars with %d storage spaces\n",
			num_workers, num_cars, num_spaces);
	fflush(stdout);

	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));

	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);

    sem_init(&sem_stage1, 0, 1);//lock in first production process so need to be initialized as 1
    sem_init(&sem_stage2, 0, 0);

    sem_init(&lock_body, 0, 1);//lock in first production process so need to be initialized as 1
	sem_init(&lock_skeleton, 0, 0);
	sem_init(&lock_engine, 0, 0);
	sem_init(&lock_chassis, 0, 0);
	sem_init(&lock_car, 0, 0);
	sem_init(&lock_battery, 0, 0);
	sem_init(&lock_tire1, 0, 0);
	sem_init(&lock_tire2, 0, 0);
	sem_init(&lock_tire3, 0, 0);
	sem_init(&lock_tire4, 0, 0);
	sem_init(&lock_window1, 0, 0);
	sem_init(&lock_window2, 0, 0);
	sem_init(&lock_window3, 0, 0);
	sem_init(&lock_window4, 0, 0);
	sem_init(&lock_window5, 0, 0);
	sem_init(&lock_window6, 0, 0);
	sem_init(&lock_window7, 0, 0);
	//initialize each semaphore created


	// prepare work_pack
	work_pack wpack;
	wpack.resource = rpack;
	wpack.tid = 0;
	wpack.totalqueuesize = num_cars * NUM_PARTS;
	wpack.jobQ = queueCreate(num_cars * NUM_PARTS);

    wpack.resource->sem_stage1 = &sem_stage1;
    wpack.resource->sem_stage2 = &sem_stage2;

    wpack.resource->lock_body = &lock_body;
    wpack.resource->lock_skeleton = &lock_skeleton;
    wpack.resource->lock_engine = &lock_engine;
    wpack.resource->lock_chassis = &lock_chassis;
    wpack.resource->lock_car = &lock_car;
    wpack.resource->lock_battery = &lock_battery;
    wpack.resource->lock_tire1 = &lock_tire1;
    wpack.resource->lock_tire2 = &lock_tire2;
    wpack.resource->lock_tire3 = &lock_tire3;
    wpack.resource->lock_tire4 = &lock_tire4;
    wpack.resource->lock_window1 = &lock_window1;
    wpack.resource->lock_window2 = &lock_window2;
    wpack.resource->lock_window3 = &lock_window3;
    wpack.resource->lock_window4 = &lock_window4;
    wpack.resource->lock_window5 = &lock_window5;
    wpack.resource->lock_window6 = &lock_window6;
    wpack.resource->lock_window7 = &lock_window7;
    //put the semaphores into wpack

	int i, j;
	// Start timing the whole process: scheduling + production
	double totalTime = omp_get_wtime();
	// Phase 1: Job scheduling, enqueue jobs' ID to jobQ
	// 8 production tasks to be done and their job ID is from 0 to 7
	// Hint: We need 7 windows and 4 tires to make a car,
	// so when i equal to WINDOW and TIRE we need to enqueue
	// 7 and 4 times respectively. Otherwise only enqueue once
	//TODO enqueue jobID to make a car
	int a=0, b=1,c=2, d=3, e=4, f=5, g=6, h=7;
	//SKELETON,ENGINE,CHASSIS,BODY,WINDOW,BODY,TIRE,BATTERY,CAR
    for (int k=0; k<num_cars; k++){
        queueEnqueue(wpack.jobQ, &d);//BODY
        queueEnqueue(wpack.jobQ, &a);//SKELETON
        queueEnqueue(wpack.jobQ, &b);//ENGINE
        queueEnqueue(wpack.jobQ, &c);//CHASSIS
        queueEnqueue(wpack.jobQ, &h);//CAR
        queueEnqueue(wpack.jobQ, &g);//BATTERY
        for (j=0; j<4; j++) queueEnqueue(wpack.jobQ, &f);//TIRE
        for (j=0; j<7; j++) queueEnqueue(wpack.jobQ, &e);//WINDOW
	}
	//enque according to the sequence: BODY, SKELETON, ENGINE, CHASSIS, CAR, BATTERY, TIRE, WINDOW
    //to ensure avoidance of deadlock and efficiency

	//implement queue.c with queuePrint function.
	if(wpack.jobQ->capacity < 100)
		queuePrint(wpack.jobQ);

	//Phase 2: Production
	int rc, jobID;
	pthread_t *threads = malloc(num_workers * sizeof(pthread_t));
    for(int j = 0; j < num_workers; j++){
        if ((rc = pthread_create(&threads[j], NULL, workerThreadFuc, &wpack))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    //use num_workers no. of threads to execute the production processes
    for (int i = 0; i < num_workers; i++) pthread_join(threads[i], NULL);

	totalTime = omp_get_wtime() - totalTime;
	reportResults(totalTime);

	destroySem();
	queueDestroy(wpack.jobQ);
	free(rpack);
	return EXIT_SUCCESS;
}

void reportResults(double production_time) {
	int *sem_value = malloc(sizeof(int));
	printf("=====Production report=====\n");
	printf("Number of workers: %d, Storage space size: %d\n", num_workers, num_spaces);
	sem_getvalue(&sem_skeleton, sem_value);
	printf("Unused Skeleton: %d\n",   *sem_value);
	sem_getvalue(&sem_engine,   sem_value);
	printf("Unused Engine: %d\n",     *sem_value);
	sem_getvalue(&sem_chassis,  sem_value);
	printf("Unused Chassis: %d\n",    *sem_value);
	sem_getvalue(&sem_body,     sem_value);
	printf("Unused Body: %d\n",       *sem_value);
	sem_getvalue(&sem_window,   sem_value);
	printf("Unused Window: %d\n",     *sem_value);
	sem_getvalue(&sem_tire,     sem_value);
	printf("Unused Tire: %d\n",       *sem_value);
	sem_getvalue(&sem_battery,  sem_value);
	printf("Unused Battery: %d\n",    *sem_value);

	sem_getvalue(&sem_space, sem_value);
	if (*sem_value < num_spaces) {
		printf("There are waste car parts!\n");
	}
	sem_getvalue(&sem_car, sem_value);
	printf("Production of %d %s done, production time: %f sec\n",
			*sem_value,
			*sem_value > 1 ? "cars" : "car",
			production_time);
	printf("==========\n");
	free(sem_value);
}

void initResourcePack(struct resource_pack *pack,
		int space_limit, int num_workers) {
	initSem();
	pack->space_limit  = space_limit;
	pack->num_workers  = num_workers;
	pack->sem_space    = &sem_space   ;
	pack->sem_worker   = &sem_worker  ;

	pack->sem_skeleton = &sem_skeleton;
	pack->sem_engine   = &sem_engine  ;
	pack->sem_chassis  = &sem_chassis ;
	pack->sem_body     = &sem_body    ;

	pack->sem_window   = &sem_window  ;
	pack->sem_tire     = &sem_tire    ;
	pack->sem_battery  = &sem_battery ;
	pack->sem_car      = &sem_car     ;
}

int destroySem(){
#if DEBUG
	printf("Destroying semaphores...\n");
#endif
	sem_destroy(&sem_worker);
	sem_destroy(&sem_space);

	sem_destroy(&sem_skeleton);
	sem_destroy(&sem_engine);
	sem_destroy(&sem_chassis);
	sem_destroy(&sem_body);

	sem_destroy(&sem_window);
	sem_destroy(&sem_tire);
	sem_destroy(&sem_battery);
	sem_destroy(&sem_car);

	sem_destroy(&sem_stage1);
    sem_destroy(&sem_stage2);

    sem_destroy(&lock_body);
    sem_destroy(&lock_skeleton);
    sem_destroy(&lock_engine);
    sem_destroy(&lock_chassis);
    sem_destroy(&lock_car);
    sem_destroy(&lock_battery);
    sem_destroy(&lock_tire1);
    sem_destroy(&lock_tire2);
    sem_destroy(&lock_tire3);
    sem_destroy(&lock_tire4);
    sem_destroy(&lock_window1);
    sem_destroy(&lock_window2);
    sem_destroy(&lock_window3);
    sem_destroy(&lock_window4);
    sem_destroy(&lock_window5);
    sem_destroy(&lock_window6);
    sem_destroy(&lock_window7);
#if DEBUG
	printf("Semaphores destroyed\n");
#endif
	return 0;
}

int initSem(){
#if DEBUG
	printf("Initiating semaphores...\n");
#endif
	sem_init(&sem_worker,   0, num_workers);
	sem_init(&sem_space,    0, num_spaces);

	sem_init(&sem_skeleton, 0, 0);
	sem_init(&sem_engine,   0, 0);
	sem_init(&sem_chassis,  0, 0);
	sem_init(&sem_body,     0, 0);

	sem_init(&sem_window,   0, 0);
	sem_init(&sem_tire,     0, 0);
	sem_init(&sem_battery,  0, 0);
	sem_init(&sem_car,      0, 0);
#if DEBUG
	printf("Init semaphores done!\n");
#endif
	return 0;
}

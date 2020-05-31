#include "worker.h"
#include <sys/types.h>

void* workerThreadFuc(void *arg){
	work_pack *wpack = (work_pack *)arg;
	int tid = wpack->tid;
	int totalqueuesize = wpack->totalqueuesize;
	queue *jobQ = wpack->jobQ;
	int jobID;
	int queuesize;
	while(queueIsEmpty(jobQ) == 0){
        queueDequeueFront(jobQ, &jobID, &queuesize);
        int rank = (totalqueuesize-queuesize)%17;
        //rank: 0: BODY,SKELETON,ENGINE,CHASSIS,CAR,BATTERY,TIRE,WINDOW
        //the rank is in a sequence the same with the queue for each production loop
        //As I distribute the jobs according to the rank, ensuring each process enter the job in same order with the queue.
        switch(rank){
            case 0://BODY
                sem_wait(wpack->resource->sem_stage1);//lock stage1
                sem_wait(wpack->resource->lock_body);//lock BODY
                sem_post(wpack->resource->lock_skeleton);//open the lock for skeleton
                work(jobID,wpack->resource,jobID);
                sem_post(wpack->resource->sem_stage2);//open lock for stage after completion of BODY
                break;

            case 1://SKELETON
                sem_wait(wpack->resource->lock_skeleton);//lock skeleton
                sem_post(wpack->resource->lock_engine);//open lock for engine
                work(jobID,wpack->resource,jobID);
                break;

            case 2://ENGINE
                sem_wait(wpack->resource->lock_engine);
                sem_post(wpack->resource->lock_chassis);
                work(jobID,wpack->resource,jobID);
                break;

            case 3://CHASSIS
                sem_wait(wpack->resource->lock_chassis);
                sem_post(wpack->resource->lock_car);
                work(jobID,wpack->resource,jobID);
                break;

            case 4://CAR
                sem_wait(wpack->resource->sem_stage2);//lock stage2
                sem_wait(wpack->resource->lock_car);//lock CAR
                sem_post(wpack->resource->lock_battery);//open lock for BATTERY
                work(jobID,wpack->resource,jobID);
                break;

            case 5://BATTERY
                sem_wait(wpack->resource->lock_battery);
                sem_post(wpack->resource->lock_tire1);
                work(jobID,wpack->resource,jobID);
                break;

            case 6://TIRE1
                sem_wait(wpack->resource->lock_tire1);
                sem_post(wpack->resource->lock_tire2);
                work(jobID,wpack->resource,jobID);
                break;

            case 7://TIRE2
                sem_wait(wpack->resource->lock_tire2);
                sem_post(wpack->resource->lock_tire3);
                work(jobID,wpack->resource,jobID);
                break;

            case 8://TIRE3
                sem_wait(wpack->resource->lock_tire3);
                sem_post(wpack->resource->lock_tire4);
                work(jobID,wpack->resource,jobID);
                break;

            case 9://TIRE4
                sem_wait(wpack->resource->lock_tire4);
                sem_post(wpack->resource->lock_window1);
                work(jobID,wpack->resource,jobID);
                break;

            case 10://WINDOW1
                sem_wait(wpack->resource->lock_window1);
                sem_post(wpack->resource->lock_window2);
                work(jobID,wpack->resource,jobID);
                break;

            case 11://WINDOW2
                sem_wait(wpack->resource->lock_window2);
                sem_post(wpack->resource->lock_window3);
                work(jobID,wpack->resource,jobID);
                break;

            case 12://WINDOW3
                sem_wait(wpack->resource->lock_window3);
                sem_post(wpack->resource->lock_window4);
                work(jobID,wpack->resource,jobID);
                break;

            case 13://WINDOW4
                sem_wait(wpack->resource->lock_window4);
                sem_post(wpack->resource->lock_window5);
                work(jobID,wpack->resource,jobID);
                break;

            case 14://WINDOW5
                sem_wait(wpack->resource->lock_window5);
                sem_post(wpack->resource->lock_window6);
                work(jobID,wpack->resource,jobID);
                break;

            case 15://WINDOW6
                sem_wait(wpack->resource->lock_window6);
                sem_post(wpack->resource->lock_window7);
                work(jobID,wpack->resource,jobID);
                break;

            case 16://WINDOW7
                sem_wait(wpack->resource->lock_window7);//lock the 7th WINDOW
                sem_post(wpack->resource->lock_body);//open lock for BODY of next loop
                work(jobID,wpack->resource,jobID);
                break;
        }
    }
    pthread_exit(NULL);
}

void work(int jobID, resource_pack *pack, int tid) {
	switch(jobID) {
		case SKELETON  :
			makeSkeleton(pack->sem_space, pack->sem_skeleton);
#if DEBUG
			printf("Worker[%d]: One car skeleton made!\n", jobID);
			fflush(stdout);
#endif
			break;
		case ENGINE    :
			makeEngine(pack->sem_space, pack->sem_engine);
#if DEBUG
			printf("Worker[%d]: One engine made!\n", tid);
			fflush(stdout);
#endif
			break;
		case CHASSIS   :
			makeChassis(pack->sem_space, pack->sem_chassis);
#if DEBUG
			printf("Worker[%d]: One chassis made!\n", tid);
			fflush(stdout);
#endif
			break;
		case BODY      :
			makeBody(pack->sem_space, pack->space_limit,
					pack->sem_body,
					pack->sem_skeleton,
					pack->sem_engine,
					pack->sem_chassis);
#if DEBUG
			printf("Worker[%d]: One body made!\n", tid);
			fflush(stdout);
#endif
			break;
		case WINDOW    :
			makeWindow(pack->sem_space, pack->sem_window);
#if DEBUG
			printf("Worker[%d]: One window made!\n", tid);
			fflush(stdout);
#endif
			break;
		case TIRE      :
			makeTire(pack->sem_space, pack->sem_tire);
#if DEBUG
			printf("Worker[%d]: One tire made!\n", tid);
			fflush(stdout);
#endif
			break;
		case BATTERY   :
			makeBattery(pack->sem_space, pack->sem_battery);
#if DEBUG
			printf("Worker[%d]: One battery pack made!\n", tid);
			fflush(stdout);
#endif
			break;
		case CAR       :
			makeCar(pack->sem_space, pack->space_limit,
					pack->sem_car,
					pack->sem_window,
					pack->sem_tire,
					pack->sem_battery,
					pack->sem_body,
					pack->sem_stage1);
#if DEBUG
			printf("Worker[%d]: One car made!\n", tid);
			fflush(stdout);
#endif
			break;
		default:
			printf("Error!! Worker[%d]: No such kind of job\n", tid);
			fflush(stdout);
			break;
	}
#if DEBUG
	printf("Worker[%d]: job %d done!\n", tid, jobID);
	fflush(stdout);
#endif
}

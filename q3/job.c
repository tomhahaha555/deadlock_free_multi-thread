#include "job.h"

void reportJobDone(sem_t *sem_worker, int num_worker) {
	int num_free_worker;
	sem_getvalue(sem_worker, &num_free_worker);
	if(num_free_worker < num_worker) {
		sem_post(sem_worker);
	}else {
		printf("Error, number of free workers exceeds num_worker\n");
		fflush(stdout);
		exit(1);
	}
}

int requestSpace(sem_t *space) {
#if DEBUG
	int num_free_space;
	sem_getvalue(space, &num_free_space);
	printf("Requesting free space, current space=%d...\n", num_free_space);
	fflush(stdout);
#endif
	sem_wait(space);
#if DEBUG
	sem_getvalue(space, &num_free_space);
	printf("Space requested, current space=%d...\n", num_free_space);
	fflush(stdout);
#endif
	return 0;
}

void releaseSpace(sem_t *space, int space_limit) {
	int num_free_space;
	sem_getvalue(space, &num_free_space);
	if(num_free_space < space_limit) {
#if DEBUG
		printf("releasing free space, current space=%d...\n", num_free_space);
		fflush(stdout);
#endif
		sem_post(space);
#if DEBUG
		sem_getvalue(space, &num_free_space);
		printf("Space released, current space=%d...\n", num_free_space);
		fflush(stdout);
#endif
	} else {
		printf("Error, releasing space that doesn't exist\n");
		fflush(stdout);
		exit(1);
	}
}

void makeItem(sem_t *space, int makeTime, sem_t* item) {
	sleep(makeTime);
	requestSpace(space);
	sem_post(item);
}

void getItem(sem_t *space, int space_limit, sem_t *item) {
	sem_wait(item);
	releaseSpace(space, space_limit);
}

void makeSkeleton(sem_t *sem_space, sem_t *sem_skeleton) {
	sleep(TIME_SKELETON);
	requestSpace(sem_space);
	sem_post(sem_skeleton);
}

void makeEngine(sem_t *sem_space, sem_t *sem_engine) {
    sleep(TIME_ENGINE);
	requestSpace(sem_space);
	sem_post(sem_engine);
}

void makeChassis(sem_t *sem_space, sem_t *sem_chassis) {
	sleep(TIME_CHASSIS);
	requestSpace(sem_space);
	sem_post(sem_chassis);
}

void makeWindow(sem_t *sem_space, sem_t *sem_window) {
	sleep(TIME_WINDOW);
	requestSpace(sem_space);
	sem_post(sem_window);
}

void makeTire(sem_t *sem_space, sem_t *sem_tire) {
	sleep(TIME_TIRE);
	requestSpace(sem_space);
	sem_post(sem_tire);
}

void makeBattery(sem_t *sem_space, sem_t *sem_battery) {
	sleep(TIME_BATTERY);
	requestSpace(sem_space);
	sem_post(sem_battery);
}

void makeBody(sem_t *sem_space, int space_limit, sem_t *sem_body,
    sem_t *sem_skeleton, sem_t *sem_engine, sem_t *sem_chassis) {

    int getskeleton=0, getengine=0, getchassis=0;
    int currentskeleton, currentengine, currentchassis;
    while (getskeleton!=1 || getengine!=1 || getchassis!=1){
        sem_getvalue(sem_skeleton, &currentskeleton);
        sem_getvalue(sem_engine, &currentengine);
        sem_getvalue(sem_chassis, &currentchassis);
        if (currentskeleton >= 1 && getskeleton!=1){
            getItem(sem_space, space_limit, sem_skeleton);
            getskeleton=1;
        }
        else if (currentengine >= 1 && getengine!=1){
            getItem(sem_space, space_limit, sem_engine);
            getengine=1;
        }
        else if (currentchassis >= 1 && getchassis!=1){
            getItem(sem_space, space_limit, sem_chassis);
            getchassis=1;
        }
    }
    //a while loop to ensure BODY gets its parts needed

	sleep(TIME_BODY);
	requestSpace(sem_space);
	sem_post(sem_body);
}

void makeCar(sem_t *sem_space, int space_limit, sem_t *sem_car,
		sem_t *sem_window, sem_t *sem_tire, sem_t *sem_battery, sem_t *sem_body, sem_t *sem_stage1) {
    int getbody=0, getbattery=0, gettire=0, getwindow=0;
    int currentbody, currentbattery ,currenttire, currentwindow;
    while (getbody!=1 || getbattery!=1 || gettire!=4 || getwindow!=7){
        sem_getvalue(sem_body, &currentbody);
        sem_getvalue(sem_battery, &currentbattery);
        sem_getvalue(sem_tire, &currenttire);
        sem_getvalue(sem_window, &currentwindow);
        if (currentbody >= 1 && getbody!=1){
            getItem(sem_space, space_limit, sem_body);
            getbody=1;
        }
        else if (currentbattery >= 1 && getbattery!=1){
            getItem(sem_space, space_limit, sem_battery);
            getbattery=1;
        }
        else if (currenttire >= 1 && gettire!=4){
            getItem(sem_space, space_limit, sem_tire);
            gettire+=1;
        }
        else if (currentwindow >= 1 && getwindow!=7){
            getItem(sem_space, space_limit, sem_window);
            getwindow+=1;
        }
    }
    //a while loop to ensure BODY gets its parts needed
    sem_post(sem_stage1);
    //open lock for stage1 of next loop
    //reason to set at this position before production of CAR and after getting all parts
    //is to utilize the production time of CAR. This is lockfree as completion of CAR doesn't
    //need a storage space.
	sleep(TIME_CAR);
	sem_post(sem_car);
}

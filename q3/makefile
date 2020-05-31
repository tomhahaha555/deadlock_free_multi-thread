OBJS = main.o job.o worker.o queue.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -fopenmp -pthread 

all: tesla_factory.out

tesla_factory.out: $(OBJS)  
	$(CC) $(CFLAGS) $(OBJS) -o tesla_factory.out ; rm *.o

main.o: definitions.h main.h main.c queue.h queue.c
	$(CC) $(CFLAGS) -c main.c

worker.o: worker.h job.h definitions.h worker.c queue.h queue.c
	$(CC) $(CFLAGS) -c worker.c

job.o: definitions.h job.h job.c
	$(CC) $(CFLAGS) -c job.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm *.out

#ifndef __PROD_CONS_H__
#define __PROD_CONS_H__

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

typedef struct sharedobject {
	FILE *rfile;
	char **line;
	int* stat1;
	int* stat;
	sem_t empty;
	pthread_mutex_t mutex;
	sem_t full;
	int p_idx;
	int c_idx;
	int blocks;
	int nprod;
	int ncons;
	int buf_size;
	int offset;
} so_t;

#define ASCII_SIZE	256

void *producer(void *arg);
void *consumer(void *arg);

#endif //!__PROD_CONS_H__
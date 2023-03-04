#include "prod_cons.h"

void *producer(void *arg) {
	// struct timeval t0, t1, dt;
	// gettimeofday(&t0, NULL);

	so_t *so = arg;

	int *ret = malloc(sizeof(int));
	FILE *rfile = so->rfile;
	int buf_size = so->buf_size;
	char *line = malloc(sizeof(*line) * buf_size);
	int blocks = so->blocks;
	ssize_t read = 0;

	while (1) {
		
		sem_wait(&(so->empty));
		pthread_mutex_lock(&(so->mutex));
		read = fread(line, 1, buf_size, rfile);

		if (read == 0) {
			so->line[so->p_idx] = NULL;
			pthread_mutex_unlock(&(so->mutex));
			sem_post(&(so->full));
			break;
		}
		so->line[so->p_idx] = strdup(line);
		so->p_idx = (so->p_idx + 1) % blocks;
		pthread_mutex_unlock(&(so->mutex));
		sem_post(&(so->full));
	}
	free(line);

	// gettimeofday(&t1, NULL);

    // timersub(&t1, &t0, &dt);

	// *ret = dt.tv_sec;

	*ret = 0;
	pthread_exit(ret);
}

void *consumer(void *arg) {
    // struct timeval t0, t1, dt;
    // gettimeofday(&t0, NULL);

	so_t *so = arg;

	//int *ret = malloc(sizeof(int));
	int *ret = malloc(sizeof(int));
	int *stat = calloc(ASCII_SIZE, sizeof(int));
	int len;
	char *line;
	int blocks = so->blocks;	
	int arr[5] = { 1, 1, 1, 1, 1 };

	while (1) {
		sem_wait(&(so->full));
		pthread_mutex_lock(&(so->mutex));

		line = so->line[so->c_idx];
		
		if (line == NULL) {
			pthread_mutex_unlock(&(so->mutex));
			sem_post(&(so->full));
			sem_post(&(so->empty));
			break;
		}
		char* copy = strdup(line);

		free(so->line[so->c_idx]);
		so->line[so->c_idx] = NULL;
		so->c_idx = (so->c_idx + 1) % blocks;

		pthread_mutex_unlock(&(so->mutex));
		sem_post(&(so->empty));

		char *cptr = copy;
		char *substr = NULL;
		char *brka = NULL;
		char *sep = "{}()[],;\" \n\t^";

		for (substr = strtok_r(cptr, sep, &brka); substr; substr = strtok_r(NULL, sep, &brka)) {

			len = strlen(cptr);

			for (int i = 0 ; i < len ; i++) {
				int c = *cptr;
				if (c < 256 && c > 1) {
					stat[c]++;
				}

				// more overhead
				for (int j = 0; j < c; j++) {
					arr[(len + j) % 5] *= c;
				}
				// more overhead

				cptr++;
			}
			cptr++;
			if (*cptr == '\0') break;

		}
	}

	// gettimeofday(&t1, NULL);

    // timersub(&t1, &t0, &dt);

	//*ret = dt.tv_sec;

	*ret = stat;
	pthread_exit(ret);
}


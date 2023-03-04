#include "prod_cons.h"
#include <pthread.h>

void *no_work_cons(void *arg) {
	so_t *so = arg;

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

	}
	pthread_exit(NULL);	
}

void *no_work_prod(void *arg) {
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
}

int main (int argc, char *argv[])
{
	pthread_t* prod;
	pthread_t* cons;
	int Nprod, Ncons;
	int buf_size;
	int rc;
	int *ret;
	int i;
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #producer #consumer #buffer_size\n");
		exit (0);
	}
	
	so_t *share = malloc(sizeof(so_t));
	memset(share, 0, sizeof(so_t));
	//share->stat = calloc(ASCII_SIZE, sizeof(*(share->stat)));
	int stat[ASCII_SIZE] = { 0, };

	if (argv[2] != NULL) {
		Nprod = atoi(argv[2]);
		if (Nprod > 4096) Nprod = 4096;
		if (Nprod == 0) Nprod = 1;
	} else Nprod = 1;
	if (argv[3] != NULL) {
		Ncons = atoi(argv[3]);
		if (Ncons > 4096) Ncons = 4096;
		if (Ncons == 0) Ncons = 1;
	} else Ncons = 1;
	if (argv[4] != NULL) {
		buf_size = atoi(argv[4]);
		//if (buf_size > 262144) buf_size = 262144;
		if (buf_size == 0) buf_size = 4096;
	} else buf_size = 4096;

	int** stats = malloc(sizeof(*stat) * Ncons);

	prod = malloc(sizeof(*prod) * Nprod);
	cons = malloc(sizeof(*cons) * Ncons);

	share->blocks = 512;
	share->nprod = Nprod;
	share->ncons = Ncons;

	FILE* rfile = fopen((char *) argv[1], "r");
	if (rfile == NULL) {
		perror("rfile");
		exit(0);
	}
	share->rfile = rfile;


//	share->rfile = malloc(sizeof(*share->rfile) * share->blocks);

	// for (int i = 0; i < share->blocks; i++) {
	// 	FILE* rfile = fopen((char *) argv[1], "r");
	// 	if (rfile == NULL) {
	// 		perror("rfile");
	// 		exit(0);
	// 	}
	// 	share->rfile[i] = rfile;
	// }

	share->line = malloc(sizeof(*(share->line)) * share->blocks);
	
	share->buf_size = buf_size;

	sem_init(&(share->empty), 0, share->nprod); 
	sem_init(&(share->full), 0, 0); 
	
	pthread_mutex_init(&(share->mutex), NULL);

	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, no_work_prod, share);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, no_work_cons, share);
	//printf("main continuing\n");

	//int p_time = 0, c_time = 0;

	for (i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void **) &ret);
		//printf("main: producer_%d joined with %d\n", i, *ret);
		//p_time += *ret;
	}

	for (i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void **) &ret);
		//printf("main: consumer_%d joined with %d\n", i, *ret);
		//c_time += *ret;
		//stats[i] = (int*)ret;
	}

	//for (i = 0; i < Ncons; i++) {
	//	for (int j = 0; j < ASCII_SIZE; j++) {
	//
	//		stat[j] += stats[i][j];
	//	}
	//}
	// for (int i = 0; i < share->blocks; i++) {
	// 	fclose(share->rfile[i]);
	// }

	//printf("p: %d c :%d\n", p_time, c_time);

	// print out distributions
	// printf("*** print out distributions *** \n");
	// printf("       A        B        C        D        E        F        G        H        I        J        K        L        M        N        O        P        Q        R        S        T        U        V        W        X        Y        Z\n");
	// // printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\n",
	// // 		share->stat['A']+share->stat['a'], share->stat['B']+share->stat['b'],  share->stat['C']+share->stat['c'],  share->stat['D']+share->stat['d'],  share->stat['E']+share->stat['e'],
	// // 		share->stat['F']+share->stat['f'], share->stat['G']+share->stat['g'],  share->stat['H']+share->stat['h'],  share->stat['I']+share->stat['i'],  share->stat['J']+share->stat['j'],
	// // 		share->stat['K']+share->stat['k'], share->stat['L']+share->stat['l'],  share->stat['M']+share->stat['m'],  share->stat['N']+share->stat['n'],  share->stat['O']+share->stat['o'],
	// // 		share->stat['P']+share->stat['p'], share->stat['Q']+share->stat['q'],  share->stat['R']+share->stat['r'],  share->stat['S']+share->stat['s'],  share->stat['T']+share->stat['t'],
	// // 		share->stat['U']+share->stat['u'], share->stat['V']+share->stat['v'],  share->stat['W']+share->stat['w'],  share->stat['X']+share->stat['x'],  share->stat['Y']+share->stat['y'],
	// // 		share->stat['Z']+share->stat['z']);

	// printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\n",
	// 		stat['A']+stat['a'], stat['B']+stat['b'],  stat['C']+stat['c'],  stat['D']+stat['d'],  stat['E']+stat['e'],
	// 		stat['F']+stat['f'], stat['G']+stat['g'],  stat['H']+stat['h'],  stat['I']+stat['i'],  stat['J']+stat['j'],
	// 		stat['K']+stat['k'], stat['L']+stat['l'],  stat['M']+stat['m'],  stat['N']+stat['n'],  stat['O']+stat['o'],
	// 		stat['P']+stat['p'], stat['Q']+stat['q'],  stat['R']+stat['r'],  stat['S']+stat['s'],  stat['T']+stat['t'],
	// 		stat['U']+stat['u'], stat['V']+stat['v'],  stat['W']+stat['w'],  stat['X']+stat['x'],  stat['Y']+stat['y'],
	// 		stat['Z']+stat['z']);

	pthread_exit(NULL);
	pthread_mutex_destroy(&(share->mutex));
    sem_destroy(&(share->empty));
    sem_destroy(&(share->full));\
	
	exit(0);
}


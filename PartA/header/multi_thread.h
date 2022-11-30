#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Create other necessary functions here

struct parameters {
	int start,end, N;
	int *matA;
	int *matB;
	int *matrix_output;
	
};

void *func(void *ptr) {
	struct parameter *p_value = (struct parameter *) ptr;
	int start = p_value->start;
	int end = p_value->end;
	int N = p_value->N;
	int *matA = p_value->matA;
	int *matB = p_value->matB;
	int *matrix_output = p_value->matrix_output;
	
	
	int sum;

	for (int j = start; j < end; j += 2) {   
		int col_1[N];
		int col_2[N];

		for (int k = 0; k < N; k++) {
			col_1[k] = matB[(k*N) + j];
			col_2[k] = matB[(k*N) + j+1];
		} 

		for (int i = 0; i < N; i += 2) {
			int sum = 0;

			for (int k = 0; k < N; k++) {
				sum = sum + matA[(i*N)+k] * col_1[k];
				sum = sum + matA[(i+1)*(N)+k] * col_1[k];
				sum = sum + matA[(i*N)+k] * col_2[k];				
				sum = sum + matA[(i+1)*(N)+k] * col_2[k];
			}

			int r = i >> 1;
			int offset = j >> 1;
			matrix_output[(r*(N>>1)) + offset] = sum; 
		}
	}
}



void multiThread(int n, int *matA, int *matB, int *matrix_output) {
	int max_no_threads = 16;
	int rows_per_thread = N / max_no_threads;

	pthread_t *threads = (pthread_t *) malloc(max_no_threads * sizeof(pthread_t));
	struct parameters *p_value = (struct parameters*) malloc(max_no_threads * sizeof(pthread_t));

	int k = 0;
	for (int i = 0; i < max_no_threads; i++) {
		p_value[i].start = k;
		p_value[i].end = k + rows_per_thread - 1;
		p_value[i].N = N;
		p_value[i].matA = matA;
		p_value[i].matB = matB;
		p_value[i].matrix_output = matrix_output;		
		k = k + rows_per_thread;
	}

	for (int i = 0; i < max_no_threads; i++) {
		pthread_create(&threads[i], NULL, func, (void *)&p_value[i]);
	}

	for (int i = 0; i < max_no_threads; i++) {
		pthread_join(threads[i], NULL );
	}    
}
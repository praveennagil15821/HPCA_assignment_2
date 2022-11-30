
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include <cuda.h>


#define N  512

_global_ void cuda_RMM(int *matA, int *matB, int *matrix_output) {
  int row_A = row;
  int col_B = col;
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
 

  if (((row %2)==0) && (row<N) && (col+1<N)) {
    int sum=0;
    
    for(int index = 0; index < N; index++) {
      sum = sum + matA[row_A * N + index] * matB[index * N + col_B];
      sum = sum + matA[(row_A+1) * N + index] * matB[index * N + col_B];
      sum = sum + matA[row_A * N + index] * matB[index * N + (col_B+1)];
      sum = sum + matA[(row_A+1) * N + index] * matB[index * N + (col_B+1)];
    }

    int row_C = row_A >> 1;
    int col_C = col_B >> 1;

    int k = row_C * (N>>1) + col_C;
    matrix_output[k] = sum;
  }
}


void gpuThread(int N, int *matA, int *matB, int* matrix_output) {  
  int max_no_threads = 32;
  int num_block = N / max_no_threads;
  dim3 threads(max_no_threads, max_no_threads);
  dim3 blocks(num_block, num_block);

  int *matA_dev, *matB_dev, *matrix_output_dev;

  
  cudaMalloc((void**)&matA_dev, N*N*sizeof(int));
  cudaMalloc((void**)&matB_dev, N*N*sizeof(int));
  cudaMalloc((void**)&matrix_output_dev, N*N/4*sizeof(int)); 
  cudaMemcpy(matA_dev, matA, total*sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(matB_dev, matB, total*sizeof(int), cudaMemcpyHostToDevice);
  cuda_RMM<<<blocks, threads>>>(matA_dev,matB_dev,matrix_output_dev);
  cudaMemcpy(matrix_output, matrix_output_dev, N*N/4*sizeof(int), cudaMemcpyDeviceToHost);

  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);
}




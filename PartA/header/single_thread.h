void singleThread(int N, int *matA, int *matB, int *output) {
  assert( N>=4 and N == ( N &~ (N-1)));
  for (int j = 0; j < N; j += 2) {
    int col_1[N], col_2[N];

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
      
      output[(r*(N>>1)) + offset] = sum;
    }
  }
}

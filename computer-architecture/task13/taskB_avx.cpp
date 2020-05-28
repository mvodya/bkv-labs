// Task13 for computer architecture subject
// Invertable matrix with SIMD (SSE version)
// by Mark Vodyanitskiy, Elena Bova, Danil Maltsev, Arkadiy Shneider
#include <iostream>
#include <iomanip>
#include <immintrin.h>
#include <stdlib.h>

using namespace std;

typedef float T;

const int N = 16;
const int SEED = 1;

inline T** alloc_matrix(int n, int m) {
  T** M = new T*[n];
  for (int i = 0; i < n; i++) M[i] = new T[m];
  return M;
}

inline __m256** matrix_packer(T** A, int n, int m) {
  __m256** MI = new __m256*[n];
  for (int i = 0; i < n; i++) {
    MI[i] = (__m256*)_mm_malloc(sizeof(__m256) * n, sizeof(__m256));
  }
  for (int i = 0; i < n; i++) {
    int k = 0;
    for (int j = 0; j < m; j+=8) {
      MI[i][k++] = _mm256_loadu_ps(&A[i][j]);
    }
  }
  return MI;
}

inline float matrix_get(__m256** MI, int i, int j) {
  float p[8] = {};
  _mm256_store_ps(&p[0], MI[i][j / 8]);
  return p[j % 8];
}

inline void matrix_fill_i (T** M, int n, int m) {
  for (int i = 0; i < n; i++)
    for (int j = n; j < m; j++) M[i][j] = (i == (j - n));
}

inline void matrix_row_swap(__m256** MI, int a, int b) {
  __m256* tmp = MI[a];
  MI[a] = MI[b];
  MI[b] = tmp;
}

inline void matrix_row2sub2mul(__m256** MI, int m, int a, int b) {
  float k = matrix_get(MI, b, a) /  matrix_get(MI, a, a);
  __m256 kI = _mm256_set1_ps(k);
  int q = 0;
  for (int i = 0; i < m; i+=8) {
    __m256 tI = _mm256_mul_ps(kI, MI[a][q]);
    MI[b][q] = _mm256_sub_ps(MI[b][q], tI);
    q++;
  }
}

inline void matrix_row_divide(__m256** MI, int m, int a, float v) {
  __m256 kI = _mm256_set1_ps(v);
  int k = 0;
  for (int i = 0; i < m; i+=8) {
    MI[a][k] = _mm256_div_ps(MI[a][k], kI);
    k++;
  }
}

void print_matrix(__m256** MI, int n, int offset = 0) {
  float p[8] = {};
  for (int i = 0; i < n; i++) {
    for (int j = offset / 8; j < (n + offset) / 8; j++) {
      _mm256_store_ps(&p[0], MI[i][j]);
      for (int k = 0; k < 8; k++) cout << setw(8) << std::setprecision(2) << p[k];
    }
    cout << endl;
  }
}

int main() {
  srand(SEED);

  T** matrix = alloc_matrix(N, 2*N);

  for (int i = 0; i < N; i++)
    for (int j = 0; j < 2*N; j++)
      matrix[i][j] = rand() % 10;
  matrix_fill_i(matrix, N, 2*N);
  
  __m256** pmatrix = matrix_packer(matrix, N, N*2);
  
  cout << "Source matrix: \n";
  print_matrix(pmatrix, N);
  cout << "\n";
  

  // Matrix reduce (by column & row)
  for (int i = 0; i < N; i++) {
    // Select none zero element and push up on first row
    for (int j = i; j < N; j++) {
      if (matrix_get(pmatrix, i, j) != 0) {
        matrix_row_swap(pmatrix, i, j);
        break;
      }
    }
    
    // Row working
    for (int j = 1 + i; j < N; j++) matrix_row2sub2mul(pmatrix, 2*N, i, j);
  }

  // Upper trinagle calculation
  for (int i = 1; i < N; i++)
    for (int j = 0; j < i; j++) matrix_row2sub2mul(pmatrix, 2*N, i, j);

  // Diagonal reduction
  for (int i = 0; i < N; i++)
    matrix_row_divide(pmatrix, 2*N, i, matrix_get(pmatrix, i, i));

  cout << "\nInvertable matrix: \n";
  print_matrix(pmatrix, N, N);
  cout << "\n";
  
  return 0;
}
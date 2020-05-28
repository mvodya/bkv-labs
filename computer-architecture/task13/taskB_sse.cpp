// Task13 for computer architecture subject
// Invertable matrix with SIMD (SSE version)
// by Mark Vodyanitskiy, Elena Bova, Danil Maltsev, Arkadiy Shneider
#include <iostream>
#include <iomanip>
#include <nmmintrin.h>

using namespace std;

typedef float T;

const int N = 16;
const int SEED = 1;

inline T** alloc_matrix(int n, int m) {
  T** M = new T*[n];
  for (int i = 0; i < n; i++) M[i] = new T[m];
  return M;
}

inline __m128** matrix_packer(T** A, int n, int m) {
  __m128** MI = new __m128*[n];
  for (int i = 0; i < n; i++) MI[i] = new __m128[m / 4];
  for (int i = 0; i < n; i++) {
    int k = 0;
    for (int j = 0; j < m; j+=4) {
      MI[i][k++] = _mm_load_ps(&A[i][j]);
    }
  }
  return MI;
}

inline float matrix_get(__m128** MI, int i, int j) {
  float p[4] = {};
  _mm_store_ps(&p[0], MI[i][j / 4]);
  return p[j % 4];
}

inline void matrix_fill_i (T** M, int n, int m) {
  for (int i = 0; i < n; i++)
    for (int j = n; j < m; j++) M[i][j] = (i == (j - n));
}

inline void matrix_row_swap(__m128** MI, int a, int b) {
  __m128* tmp = MI[a];
  MI[a] = MI[b];
  MI[b] = tmp;
}

inline void matrix_row2sub2mul(__m128** MI, int m, int a, int b) {
  float k = matrix_get(MI, b, a) /  matrix_get(MI, a, a);
  __m128 kI = _mm_set1_ps(k);
  int q = 0;
  for (int i = 0; i < m; i+=4) {
    __m128 tI = _mm_mul_ps(kI, MI[a][q]);
    MI[b][q] = _mm_sub_ps(MI[b][q], tI);
    q++;
  }
}

inline void matrix_row_divide(__m128** MI, int m, int a, float v) {
  __m128 kI = _mm_set1_ps(v);
  int k = 0;
  for (int i = 0; i < m; i+=4) {
    MI[a][k] = _mm_div_ps(MI[a][k], kI);
    k++;
  }
}

void print_matrix(__m128** MI, int n, int offset = 0) {
  float p[4] = {};
  for (int i = 0; i < n; i++) {
    for (int j = offset / 4; j < (n + offset) / 4; j++) {
      _mm_store_ps(&p[0], MI[i][j]);
      for (int k = 0; k < 4; k++) cout << setw(8) << std::setprecision(2) << p[k];
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

  __m128** pmatrix = matrix_packer(matrix, N, N*2);

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
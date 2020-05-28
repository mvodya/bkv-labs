#include <iostream>
#include <iomanip>

#define FOR_I_J(n, m) for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)

using namespace std;

typedef float T;

#ifdef MATRIX_SIZE
const int N = MATRIX_SIZE;
#else
const int N = 16;
#endif
const int SEED = 1;

inline T** alloc_matrix(int n, int m) {
  T** M = new T*[n];
  for (int i = 0; i < n; i++) M[i] = new T[m];
  return M;
}

inline void matrix_fill_i (T** M, int n, int m) {
  for (int i = 0; i < n; i++)
    for (int j = n; j < m; j++) M[i][j] = (i == (j - n));
}

inline void matrix_row_swap(T** M, int a, int b) {
  T *tmp = M[a];
  M[a] = M[b];
  M[b] = tmp;
}

inline void matrix_row2sub2mul(T** M, int m, int a, int b) {
  float k = (M[b][a] / M[a][a]);
  for (int i = 0; i < m; i++) M[b][i] -= M[a][i] * k;
}

inline void matrix_row_divide(T** M, int m, int a, float v) {
  for (int i = 0; i < m; i++) M[a][i] /= v;
}

void print_matrix(T** M, int n, int offset = 0) {
  for (int i = 0; i < n; i++) {
    for (int j = offset; j < n + offset; j++) cout << setw(8) << std::setprecision(2) << M[i][j];
    cout << endl;
  }
}

int main() {
  srand(SEED);

  T** matrix = alloc_matrix(N, 2*N);

  FOR_I_J(N, N) matrix[i][j] = rand() % 10;
  matrix_fill_i(matrix, N, 2*N);

  cout << "Source matrix: \n";
  print_matrix(matrix, N);

  // Matrix reduce (by column & row)
  for (int i = 0; i < N; i++) {
    // Select none zero element and push up on first row
    for (int j = i; j < N; j++)
      if (matrix[i][j] != 0) {
        matrix_row_swap(matrix, i, j);
        break;
      }
    
    // Row working
    for (int j = 1 + i; j < N; j++) matrix_row2sub2mul(matrix, 2*N, i, j);
  }

  // Upper trinagle calculation
  for (int i = 1; i < N; i++)
    for (int j = 0; j < i; j++)
      matrix_row2sub2mul(matrix, 2*N, i, j);

  // Diagonal reduction
  for (int i = 0; i < N; i++) matrix_row_divide(matrix, 2*N, i, matrix[i][i]);

  cout << "\nInvertable matrix: \n";
  print_matrix(matrix, N, N);

  return 0;
}
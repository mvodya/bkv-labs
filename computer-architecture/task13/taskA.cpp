#include <iomanip>
#include <iostream>

#define FOR_I_J(n) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)

using namespace std;

typedef float T;
const int N = 10;
const int SEED = 1;

inline T** alloc_matrix(int n) {
  T** M = new T*[n];
  for (int i = 0; i < n; i++) M[i] = new T[n];
  return M;
}

inline void dealloc_matrix(T** M, int n) {
  for (int i = 0; i < n; i++) delete[] M[i];
  delete[] M;
}

void print_matrix(T** M, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) cout << setw(8) << std::setprecision(2) << M[i][j];
    cout << endl;
  }
}

inline int adj_minor(T** M, int n, int mi, int mj);

inline int adj_add(T** M, int n, int mi, int mj) {
  return ((mi + mj) % 2 == 0 ? 1 : -1) * adj_minor(M, n, mi, mj);
}

int determinant(T** M, int n) {
  int d = 0;

  if (n == 2) {
    d = (M[0][0] * M[1][1]) - (M[0][1] * M[1][0]);
  } else {
    for (int j = 0; j < n; j++) {
      d += (j % 2 == 0 ? 1 : -1) * M[0][j] * adj_minor(M, n, 0, j);
    }
  }
  return d;
}

inline int adj_minor(T** M, int n, int mi, int mj) {
  n--;
  T** A = alloc_matrix(n);
  int di = 0, dj = 0;
  for (int i = 0; i < n; i++) {
    if (i == mi) di = 1;
    dj = 0;
    for (int j = 0; j < n; j++) {
      if (j == mj) dj = 1;
      A[i][j] = M[i + di][j + dj];
    }
  }
  int d = determinant(A, n);
  dealloc_matrix(A, n);
  return d;
}

T** adjugate_matrix(T** M, int n) {
  T** C = alloc_matrix(n);
  FOR_I_J(n) C[i][j] = adj_add(M, n, j, i);
  return C;
}

T** invertable_matrix(T** M, int n) {
  int d = determinant(M, n);
  T** A = adjugate_matrix(M, n);
  FOR_I_J(n) A[i][j] /= d;
  return A;
}

int main() {
  srand(SEED);

  T** matrix = alloc_matrix(N);

  FOR_I_J(N) matrix[i][j] = rand() % 10;

  cout << "Source matrix: \n";
  print_matrix(matrix, N);

  T** i_matrix = invertable_matrix(matrix, N);
  cout << "\nInvertable matrix: \n";
  print_matrix(i_matrix, N);

  return 0;
}
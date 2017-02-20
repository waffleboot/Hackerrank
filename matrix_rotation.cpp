
#include <iostream>
#include <fstream>

using namespace std;

int M, N, R, D;
int matrix[300][300];

void rotate(int depth) {
  const int y = M-depth-1;
  const int x = N-depth-1;
  const int t = matrix[depth][depth];
  for (int j = depth; j < x; ++j) {
    matrix[depth][j] = matrix[depth][j+1];
  }
  for (int i = depth; i < y; ++i) {
    matrix[i][x] = matrix[i+1][x];
  }
  for (int j = x; j > depth; --j) {
    matrix[y][j] = matrix[y][j-1];
  }
  for (int i = y; i > depth; --i) {
    matrix[i][depth] = matrix[i-1][depth];
  }
  matrix[depth+1][depth] = t;
}

void print() {
  cout << "------------------\n";
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << matrix[i][j] << ' ';
    }
    cout << '\n';
  }
}

int main(int argc, const char * argv[]) {
  ifstream fis("../../matrix/test.txt");
  fis >> M; fis >> N; fis >> R;
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      fis >> matrix[i][j];
    }
  }
  print();
  D = min(M,N) / 2;
  for (int d = 0; d < D; ++d) {
    int _R = R % (2*(M+N)-8*d-4);
    for (int r = 0; r < _R; ++r) {
      rotate(d);
    }
  }
  print();
  return 0;
}

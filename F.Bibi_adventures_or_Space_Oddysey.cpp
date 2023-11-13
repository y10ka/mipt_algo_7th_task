#include <iostream>
#include <vector>

using IntVector = std::vector<int>;
using CubeVector = std::vector<std::vector<IntVector>>;

struct Cube : public CubeVector {
  explicit Cube(int n) : CubeVector(n, std::vector<IntVector>(n, IntVector(n, 0))) {
  }
};

struct CubeFenwickTree {
  int size;
  Cube data;
  explicit CubeFenwickTree(int n) : size(n), data(n + 1) {
  }
  void Update(int x, int y, int z, int delta) {
    for (int i = x; i < size + 1; i = (i | (i + 1))) {
      for (int j = y; j < size + 1; j = (j | (j + 1))) {
        for (int k = z; k < size + 1; k = (k | (k + 1))) {
          data[i][j][k] += delta;
        }
      }
    }
  }
  int GetPrefix(int x, int y, int z) {
    int result = 0;
    for (int i = x; i > 0; i = (i & (i + 1)) - 1) {
      for (int j = y; j > 0; j = (j & (j + 1)) - 1) {
        for (int k = z; k > 0; k = (k & (k + 1)) - 1) {
          result += data[i][j][k];
        }
      }
    }
    return result;
  }
  int Query(int x1, int y1, int z1, int x2, int y2, int z2) {
    int result = GetPrefix(x2, y2, z2);
    result -= GetPrefix(x2, y2, z1) + GetPrefix(x2, y1, z2) + GetPrefix(x1, y2, z2);
    result += GetPrefix(x2, y1, z1) + GetPrefix(x1, y2, z1) + GetPrefix(x1, y1, z2);
    return result - GetPrefix(x1, y1, z1);
  }
};

int main() {
  int n = 0;
  std::cin >> n;
  CubeFenwickTree tree(n);
  int query = 0;
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;
  int z1 = 0;
  int z2 = 0;
  int delta = 0;
  while (query != 3) {
    std::cin >> query;
    if (query == 1) {
      std::cin >> x1 >> y1 >> z1 >> delta;
      tree.Update(++x1, ++y1, ++z1, delta);
    } else if (query == 2) {
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      std::cout << tree.Query(x1, y1, z1, ++x2, ++y2, ++z2) << '\n';
    }
  }
}
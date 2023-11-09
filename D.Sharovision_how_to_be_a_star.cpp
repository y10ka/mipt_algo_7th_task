#include <iostream>
#include <vector>
#include <numeric>

using IntVector = std::vector<int>;
using IntMatrix = std::vector<std::vector<int>>;
int inf = 1000000000;

int MaxPowerOfTwo(int border) {
  int pwr = 0;
  int two = 1;
  while (two < border) {
    ++pwr;
    two *= 2;
  }
  return (two == border ? pwr : --pwr);
}

int PwrOfTwo(int argument) {
  int two = 1;
  while (argument--) {
    two *= 2;
  }
  return two;
}

struct GcdSparseTable {
  IntMatrix table;
  explicit GcdSparseTable(const IntVector& array) {
    auto size = static_cast<int>(array.size());
    int k = MaxPowerOfTwo(size);
    table = IntMatrix(size, IntVector(k + 1, -inf));
    for (int i = 0; i < size; ++i) {
      table[i][0] = array[i];
    }
    int width = 1;
    for (int i = 1; i <= k; ++i) {
      width *= 2;
      for (int j = 0; j < size - width + 1; ++j) {
        table[j][i] = std::gcd(table[j][i - 1], table[j + width / 2][i - 1]);
      }
    }
  }
  int Gcd(int left, int right) {
    int pwr = MaxPowerOfTwo(right - left);
    return (std::gcd(table[left][pwr], table[right - PwrOfTwo(pwr)][pwr]));
  }
};

void Input(IntVector& vector, int size) {
  for (int i = 0; i < size; ++i) {
    std::cin >> vector[i];
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n = 0;
  int q = 0;
  std::cin >> n;
  IntVector smileballs(n);
  Input(smileballs, n);
  std::cin >> q;
  GcdSparseTable table(smileballs);
  int left = 0;
  int right = 0;
  for (int i = 0; i < q; ++i) {
    std::cin >> left >> right;
    std::cout << table.Gcd(left - 1, right) << '\n';
  }
}
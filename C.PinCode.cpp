// C

#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;
using IntMatrix = std::vector<std::vector<int64_t>>;
int64_t inf = 1000000000;

int MaxPowerOfTwo(int64_t border) {
  int pwr = 0;
  int64_t two = 1;
  while (two < border) {
    ++pwr;
    two *= 2;
  }
  return (two == border ? pwr : --pwr);
}

int64_t PwrOfTwo(int64_t argument) {
  int64_t two = 1;
  while (argument--) {
    two *= 2;
  }
  return two;
}

struct MinSparseTable {
  IntMatrix table;
  explicit MinSparseTable(IntVector massive) {
    auto size = static_cast<int64_t>(massive.size());
    int k = MaxPowerOfTwo(size);
    table = IntMatrix(size, IntVector(k + 1, inf));
    for (int64_t i = 0; i < size; ++i) {
      table[i][0] = massive[i];
    }
    int64_t width = 1;
    for (int i = 1; i <= k; ++i) {
      width *= 2;
      for (int j = 0; j < size - width + 1; ++j) {
        table[j][i] = std::min(table[j][i - 1], table[j + width / 2][i - 1]);
      }
    }
  }
  int64_t Min(int64_t left, int64_t right) {
    int64_t pwr = MaxPowerOfTwo(right - left);
    return (std::min(table[left][pwr], table[right - PwrOfTwo(pwr)][pwr]));
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int64_t n = 0;
  int64_t q = 0;
  std::cin >> n >> q;
  IntVector smileballs(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> smileballs[i];
  }
  MinSparseTable table(smileballs);
  int64_t left = 0;
  int64_t right = 0;
  for (int64_t i = 0; i < q; ++i) {
    std::cin >> left >> right;
    std::cout << table.Min(left - 1, right) << '\n';
  }
}

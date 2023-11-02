#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;
int64_t zero = 0;

struct BinaryIndexedTree {
  int64_t size;
  IntVector data;
  IntVector massive;
  explicit BinaryIndexedTree(int64_t n) : size(n), data(size + 1, 0), massive(size + 1, 0) {
  }
  void Input(int64_t n) {
    IntVector temp_prefix(n + 1, 0);
    for (int64_t idx = 1; idx < n + 1; ++idx) {
      std::cin >> massive[idx];
      temp_prefix[idx] = massive[idx];
      temp_prefix[idx] += temp_prefix[idx - 1];
    }
    for (int64_t idx = 1; idx < n + 1; ++idx) {
      data[idx] = temp_prefix[idx] - temp_prefix[std::max((idx & (idx + 1)) - 1, zero)];
    }
  }
  void Update(int64_t pos, int64_t updated) {
    int64_t delta = updated - massive[pos];
    massive[pos] = updated;
    for (int64_t i = pos; i < size + 1; i = (i | (i + 1))) {
      data[i] += delta;
    }
  }
  int64_t GetPrefix(int64_t idx) {
    int64_t sum = 0;
    for (int64_t i = idx; i > 0; i = (i & (i + 1)) - 1) {
      sum += data[i];
    }
    return sum;
  }
  int64_t Quiery(int64_t left, int64_t right) {
    return GetPrefix(right) - GetPrefix(left - 1);
  }
};

int main() {
  int n = 0;
  std::cin >> n;
  BinaryIndexedTree tree(n);
  tree.Input(n);
  int m = 0;
  std::cin >> m;
  char quiery = '0';
  int64_t left = 0;
  int64_t right = 0;
  for (int i = 0; i < m; ++i) {
    std::cin >> quiery >> left >> right;
    if (quiery == 'u') {
      tree.Update(left, right);
      continue;
    }
    std::cout << tree.Quiery(left, right) << ' ';
  }
}
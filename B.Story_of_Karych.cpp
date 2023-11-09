#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;
int64_t inf = 10000000000;

IntVector BuildPrefix(int64_t n) {
  IntVector prefix_sum(n + 1, 0);
  for (int64_t i = 1; i <= n; ++i) {
    std::cin >> prefix_sum[i];
    prefix_sum[i] += prefix_sum[i - 1];
  }
  return prefix_sum;
}

int64_t ArgMin(const IntVector& array, int64_t border) {
  auto min = inf;
  int64_t min_idx = 0;
  for (int64_t i = 0; i < border; ++i) {
    if (array[i] < min) {
      min = array[i];
      min_idx = i;
    }
  }
  return min_idx;
}

void FindMax(const IntVector& array) {
  auto size = static_cast<int64_t>(array.size());
  int64_t min = 0;
  int64_t max = array[1];
  int64_t left = 0;
  int64_t cur_left = 0;
  int64_t right = 1;
  for (int64_t i = 1; i < size; ++i) {
    if (array[i - 1] < min) {
      min = array[i - 1];
      cur_left = i - 1;
    }
    if (array[i] - min > max) {
      max = array[i] - min;
      right = i;
      left = cur_left;
    }
  }
  std::cout << left + 1 << ' ' << right << ' ' << max;
}

int main() {
  int64_t n = 0;
  std::cin >> n;
  auto prefix = BuildPrefix(n);
  FindMax(prefix);
}
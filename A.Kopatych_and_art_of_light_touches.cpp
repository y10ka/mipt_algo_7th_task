#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;

IntVector BuildPrefix(int64_t n) {
  IntVector prefix_sum(n + 1, 0);
  for (int64_t i = 1; i <= n; ++i) {
    std::cin >> prefix_sum[i];
    prefix_sum[i] += prefix_sum[i - 1];
  }
  return prefix_sum;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int64_t n = 0;
  std::cin >> n;
  auto prefix_sum = BuildPrefix(n);
  int64_t k = 0;
  int64_t l = 0;
  int64_t r = 0;
  std::cin >> k;
  for (int64_t i = 0; i < k; ++i) {
    std::cin >> l >> r;
    std::cout << prefix_sum[r] - prefix_sum[l - 1] << ' ';
  }
}
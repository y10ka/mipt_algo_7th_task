#include <iostream>
#include <vector>

using IntVector = std::vector<uint32_t>;

int64_t PowerOfTwo(uint32_t degree) {
  int64_t two = 1;
  for (uint32_t i = 0; i < degree; ++i) {
    two *= 2;
  }
  return two;
}

int64_t module = PowerOfTwo(32);

uint32_t NextRand(uint32_t& cur, const uint32_t& a, const uint32_t& b) {
  cur = cur * a + b;
  return (cur >> 8);
}

int main() {
  uint32_t m = 0;
  uint32_t q = 0;
  uint32_t a = 0;
  uint32_t b = 0;
  std::cin >> m >> q >> a >> b;
  uint32_t size = PowerOfTwo(24);
  IntVector diffs(size + 1, 0);
  uint32_t cur = 0;
  int64_t add = 0;
  uint32_t l = 0;
  uint32_t r = 0;
  for (uint32_t i = 0; i < m; ++i) {
    add = NextRand(cur, a, b);
    l = NextRand(cur, a, b);
    r = NextRand(cur, a, b);
    if (l > r) {
      std::swap(l, r);
    }
    diffs[l] += add;
    diffs[r + 1] -= add;
  }
  IntVector prefix(size + 1, 0);
  uint32_t idx_value = diffs[0];
  uint32_t sum = idx_value;
  for (int64_t i = 1; i < size + 1; ++i) {
    prefix[i - 1] = sum;
    idx_value += diffs[i];
    sum += idx_value;
  }
  uint32_t result = 0;
  for (uint32_t i = 0; i < q; ++i) {
    l = NextRand(cur, a, b);
    r = NextRand(cur, a, b);
    if (l > r) {
      std::swap(l, r);
    }
    result += prefix[r];
    result -= (l ? prefix[l - 1] : 0);
  }
  std::cout << (result < 0 ? module + result : result);
}
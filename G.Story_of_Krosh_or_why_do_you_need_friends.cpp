#include <iostream>
#include <vector>

struct TreePair;
struct MaxSegmentTree;

using IntPair = std::pair<int, int>;
using PairVector = std::vector<TreePair>;

struct TreePair {
  int value;
  int count;
  TreePair() : value(-50000), count(0) {
  }
  TreePair(int value, int count) : value(value), count(count) {
  }
  TreePair operator+(const TreePair& other) {
    if (value == other.value) {
      int count_temp = count + other.count;
      return {value, count_temp};
    }
    if (value < other.value) {
      return other;
    }
    return *this;
  }
};

int MinPwr(int value) {
  if (value < 1) {
    return 0;
  }
  int border = 1;
  while (border < value) {
    border *= 2;
  }
  return border;
}

struct MaxSegmentTree {
  PairVector tree;
  int size;
  MaxSegmentTree() : tree(), size(0) {
  }
  explicit MaxSegmentTree(int n) : tree(2 * MinPwr(n) - 1), size(static_cast<int>(tree.size()) / 2 + 1) {
  }
  void Input(int n) {
    int value = 0;
    for (int i = 0; i < n; ++i) {
      std::cin >> value;
      tree[i + size - 1] = {value, 1};
    }
    for (int i = Size() - 2; i >= 0; --i) {
      tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }
  }
  IntPair Borders(int idx) {
    int left(idx);
    int right(idx);
    while (2 * idx - size + 2 < 0) {
      idx *= 2;
      left *= 2;
      right *= 2;
      ++idx;
      ++left;
      right += 2;
    }
    return {left - size + 1, right - size + 1};
  }
  int Size() {
    return size;
  }
  TreePair Query(int left_border, int right_border) {
    int left = left_border + size - 1;
    int right = right_border + size - 1;
    if (right <= left) {
      return {-50000, 0};
    }
    int temp_size = 1;
    while ((left % 2) && (temp_size * 2 <= right_border - left_border)) {
      left = (left - 1) / 2;
      temp_size *= 2;
    }
    TreePair result = tree[left];
    return (result + Query(left_border + temp_size, right_border));
  }
};

int main() {
  int n = 0;
  std::cin >> n;
  MaxSegmentTree tree(n);
  tree.Input(n);
  int k = 0;
  std::cin >> k;
  int left = 0;
  int right = 0;
  for (int i = 0; i < k; ++i) {
    std::cin >> left >> right;
    auto answer = tree.Query(left - 1, right);
    std::cout << answer.value << ' ' << answer.count << '\n';
  }
}
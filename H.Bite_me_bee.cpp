#include <iostream>
#include <vector>

struct TreePair;
struct MaxSegmentTree;

using IntPair = std::pair<int, int>;
using PairVector = std::vector<TreePair>;
int inf = 1000000;

bool SubSegment(const IntPair& sub, const IntPair& segment) {
  return (segment.first <= sub.first && sub.second <= segment.second);
}

struct TreePair {
  int value;
  int promise;
  TreePair() : value(-50000), promise(0) {
  }
  TreePair(int value, int promise) : value(value), promise(promise) {
  }
  TreePair operator+(const TreePair& other) {
    if (value == other.value) {
      return {value, promise + other.promise};
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
      tree[i + size - 1] = {value, 0};
    }
    for (int i = Size() - 2; i >= 0; --i) {
      tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }
  }
  IntPair Borders(int idx) {
    int left(idx);
    int right(idx);
    while (idx - size + 1 < 0) {
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
  void Update(int left_border, int right_border, int delta, int idx = 0) {
    IntPair borders = Borders(idx);
    if (SubSegment(borders, {left_border, right_border})) {
      tree[idx].promise += delta;
      return;
    }
    if ((borders.second < left_border) || (right_border < borders.first)) {
      return;
    }
    Update(left_border, right_border, delta, idx * 2 + 1);
    Update(left_border, right_border, delta, idx * 2 + 2);
    tree[idx].value = std::max(tree[idx * 2 + 1].value + tree[idx * 2 + 1].promise,
                               tree[idx * 2 + 2].value + tree[idx * 2 + 2].promise);
  }
  int Query(int left_border, int right_border, int idx = 0) {
    IntPair borders = Borders(idx);
    if (SubSegment(borders, {left_border, right_border})) {
      return (tree[idx].value + tree[idx].promise);
    }
    if (borders.second < left_border || right_border < borders.first) {
      return -inf;
    }
    tree[idx * 2 + 1].promise += tree[idx].promise;
    tree[idx * 2 + 2].promise += tree[idx].promise;
    tree[idx].value += tree[idx].promise;
    tree[idx].promise = 0;
    return std::max(Query(left_border, right_border, idx * 2 + 1), Query(left_border, right_border, idx * 2 + 2));
  }
};

int main() {
  int n = 0;
  std::cin >> n;
  MaxSegmentTree tree(n);
  tree.Input(n);
  int k = 0;
  std::cin >> k;
  char query = '0';
  int left = 0;
  int right = 0;
  int delta = 0;
  for (int i = 0; i < k; ++i) {
    std::cin >> query >> left >> right;
    if (query == 'm') {
      std::cout << tree.Query(--left, --right) << ' ';
    } else if (query == 'a') {
      std::cin >> delta;
      tree.Update(--left, --right, delta);
    }
  }
}
#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;
using IntMatrix = std::vector<IntVector>;
using IntPair = std::pair<int64_t, int64_t>;

bool SubSegment(const IntPair& sub, const IntPair& seg) {
  return (seg.first <= sub.first && sub.second <= seg.second);
}

int64_t PwrOfTwo(int x) {
  return (1 << x);
}

int64_t neutral = PwrOfTwo(30) * 2;

int64_t MinPwr(int64_t value) {
  int64_t two = 1;
  while (two < value) {
    two *= 2;
  }
  return two;
}

struct SegmentTree2D {
  IntMatrix field;
  int64_t size_1;
  int64_t size_2;
  SegmentTree2D(int64_t m, int64_t n)
      : field(MinPwr(m) * 2 - 1, IntVector(MinPwr(n) * 2 - 1, neutral))
      , size_1(static_cast<int64_t>(field.size()) / 2 + 1)
      , size_2(static_cast<int64_t>(field[0].size()) / 2 + 1) {
  }
  void Input(int64_t m, int64_t n) {
    for (int64_t i = 0; i < m; ++i) {
      for (int64_t j = 0; j < n; ++j) {
        std::cin >> field[i + size_1 - 1][j + size_2 - 1];
      }
      for (int64_t j = size_2 - 2; j >= 0; --j) {
        field[i + size_1 - 1][j] = std::min(field[i + size_1 - 1][j * 2 + 1], field[i + size_1 - 1][j * 2 + 2]);
      }
    }
    for (int64_t i = size_1 - 2; i >= 0; --i) {
      for (int64_t j = 0; j < size_2 * 2 - 1; ++j) {
        field[i][j] = std::min(field[i * 2 + 1][j], field[i * 2 + 2][j]);
      }
    }
  }
  IntPair BordersX(int64_t idx) {
    int64_t size = 1;
    while (idx < size_1 - 1) {
      idx = idx * 2 + 1;
      size *= 2;
    }
    return {idx - size_1 + 1, idx - size_1 + size};
  }
  IntPair BordersY(int64_t idx) {
    int64_t size = 1;
    while (idx < size_2 - 1) {
      idx = idx * 2 + 1;
      size *= 2;
    }
    return {idx - size_2 + 1, idx - size_2 + size};
  }
  void XIndexes(IntVector& x_indexes, int64_t x1, int64_t x2, int64_t idx = 0) {
    auto borders = BordersX(idx);
    if (x2 < borders.first || borders.second < x1) {
      return;
    }
    if (SubSegment(borders, {x1, x2})) {
      x_indexes.push_back(idx);
      return;
    }
    XIndexes(x_indexes, x1, x2, idx * 2 + 1);
    XIndexes(x_indexes, x1, x2, idx * 2 + 2);
  }
  void YIndexes(IntVector& y_indexes, int64_t y1, int64_t y2, int64_t idx = 0) {
    auto borders = BordersY(idx);
    if (y2 < borders.first || borders.second < y1) {
      return;
    }
    if (SubSegment(borders, {y1, y2})) {
      y_indexes.push_back(idx);
      return;
    }
    YIndexes(y_indexes, y1, y2, idx * 2 + 1);
    YIndexes(y_indexes, y1, y2, idx * 2 + 2);
  }
  int64_t Query(int64_t x1, int64_t x2, int64_t y1, int64_t y2) {
    IntVector x_indexes(0);
    IntVector y_indexes(0);
    XIndexes(x_indexes, x1, x2);
    YIndexes(y_indexes, y1, y2);
    int64_t res = neutral;
    for (auto i : x_indexes) {
      for (auto j : y_indexes) {
        res = std::min(field[i][j], res);
      }
    }
    return res;
  }
};

int main() {
  int64_t m = 0;
  int64_t n = 0;
  std::cin >> m >> n;
  SegmentTree2D tree(m, n);
  tree.Input(m, n);
  int64_t q = 0;
  std::cin >> q;
  int64_t x1 = 0;
  int64_t x2 = 0;
  int64_t y1 = 0;
  int64_t y2 = 0;
  for (int64_t i = 0; i < q; ++i) {
    std::cin >> x1 >> y1 >> x2 >> y2;
    std::cout << tree.Query(--x1, --x2, --y1, --y2) << '\n';
  }
}
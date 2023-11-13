#include <iostream>
#include <vector>
#include <string>

int MinPwr(size_t value) {
  size_t two = 1;
  while (two < value) {
    two *= 2;
  }
  return static_cast<int>(two);
}

struct Node {
  int pairs;
  int unused_open;
  int unused_close;
  Node() : pairs(0), unused_open(0), unused_close(0) {
  }
  Node(int pairs, int unused_open, int unused_close)
      : pairs(pairs), unused_open(unused_open), unused_close(unused_close) {
  }
  Node operator+(const Node& other) const {
    Node result;
    int new_pairs = std::min(unused_open, other.unused_close);
    result.pairs = pairs + other.pairs + new_pairs;
    result.unused_open = unused_open + other.unused_open - new_pairs;
    result.unused_close = unused_close + other.unused_close - new_pairs;
    return result;
  }
};

Node neutral(0, 0, 0);

struct BraceSegmentTree {
  int size;
  std::vector<Node> tree;
  explicit BraceSegmentTree(const std::string& seq) : size(MinPwr(seq.size())), tree(size * 2 - 1) {
    int seq_size = static_cast<int>(seq.size());
    for (int i = 0; i < seq_size; ++i) {
      tree[i + size - 1].unused_open = (seq[i] == '(' ? 1 : 0);
      tree[i + size - 1].unused_close = (seq[i] == ')' ? 1 : 0);
    }
    for (int i = size - 2; i >= 0; --i) {
      tree[i] = tree[i * 2 + 1] + tree[i * 2 + 2];
    }
  }
  std::pair<int, int> Borders(int idx) const {
    int sub_size = 1;
    while (idx < size - 1) {
      idx = idx * 2 + 1;
      sub_size *= 2;
    }
    return {idx - size + 1, idx + sub_size - size};
  }
  bool SubSegment(const std::pair<int, int>& sub, const std::pair<int, int>& seg) const {
    return (seg.first <= sub.first && sub.second <= seg.second);
  }
  Node Query(int left, int right, int idx, int left_b, int right_b) const {
    if (left <= left_b && right_b <= right) {
      return tree[idx];
    }
    if (right_b < left || right < left_b) {
      return neutral;
    }
    return Query(left, right, idx * 2 + 1, left_b, left_b + (right_b - left_b) / 2) +
           Query(left, right, idx * 2 + 2, left_b + (right_b - left_b) / 2 + 1, right_b);
  }
  int MaxRightSequence(int left, int right) const {
    return Query(left, right, 0, 0, size - 1).pairs * 2;
  }
  void Print() const {
    for (int i = 0; i < size * 2 - 1; ++i) {
      std::cout << "{" << tree[i].pairs << ',' << tree[i].unused_open << ',' << tree[i].unused_close << "} ";
    }
    std::cout << '\n';
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string sequence;
  int q = 0;
  std::cin >> sequence;
  std::cin >> q;
  BraceSegmentTree tree(sequence);
  int left = 0;
  int right = 0;
  for (int i = 0; i < q; ++i) {
    std::cin >> left >> right;
    std::cout << tree.MaxRightSequence(--left, --right) << '\n';
  }
}
#include <iostream>
#include <vector>
#include <string>

int64_t neutral = 11111111111;

struct Node {
  int64_t value;
  int64_t rank;
  int64_t result;
  bool reversed;
  Node* left;
  Node* right;
  Node* parent;
  Node() = default;
  explicit Node(int64_t value)
      : value(value), rank(1), result(value), reversed(false), left(nullptr), right(nullptr), parent(nullptr) {
  }
};

void Delete(Node* node) {
  if (!node) {
    return;
  }
  Delete(node->left);
  Delete(node->right);
  node->left = nullptr;
  node->right = nullptr;
  node->parent = nullptr;
  delete node;
}

struct Treap {
  Node* root;
  Treap() : root(nullptr) {
  }
  explicit Treap(Node* ptr) : root(ptr) {
  }
  ~Treap() {
    Delete(root);
  }
};

void FixNode(Node* node) {
  if (!node) {
    return;
  }
  node->result = std::min(std::min(node->value, (node->left ? node->left->result : neutral)),
                          (node->right ? node->right->result : neutral));
  node->rank = (node->left ? node->left->rank : 0) + (node->right ? node->right->rank : 0) + 1;
}

void PushReverse(Node* node) {
  if (!node) {
    return;
  }
  if (node->reversed) {
    std::swap(node->left, node->right);
    node->reversed = false;
    if (node->left) {
      node->left->reversed = !(node->left->reversed);
    }
    if (node->right) {
      node->right->reversed = !(node->right->reversed);
    }
  }
}

Node* Merge(Node* tree_1, Node* tree_2) {
  if (!tree_1) {
    return tree_2;
  }
  if (!tree_2) {
    return tree_1;
  }
  if (tree_2->value < tree_1->value) {
    PushReverse(tree_2);
    tree_2->left = Merge(tree_1, tree_2->left);
    tree_2->left->parent = tree_2;
    FixNode(tree_2);
    return tree_2;
  }
  PushReverse(tree_1);
  tree_1->right = Merge(tree_1->right, tree_2);
  tree_1->right->parent = tree_1;
  FixNode(tree_1);
  return tree_1;
}
Treap Merge(Treap tree_1, Treap tree_2) {
  return Treap(Merge(tree_1.root, tree_2.root));
}

std::pair<Node*, Node*> Split(Node* tree, int64_t x0) {
  if (!tree) {
    return {nullptr, nullptr};
  }
  PushReverse(tree);
  if ((tree->left ? tree->left->rank : 0) < x0) {
    auto t1_t2 = Split(tree->right, x0 - (tree->rank - (tree->right ? tree->right->rank : 0)));
    Node* tree_1 = t1_t2.first;
    Node* tree_2 = t1_t2.second;
    tree->right = tree_1;
    if (tree_1) {
      tree_1->parent = tree;
    }
    FixNode(tree);
    if (tree_2) {
      tree_2->parent = nullptr;
    }
    FixNode(tree_2);
    return {tree, tree_2};
  }
  auto t1_t2 = Split(tree->left, x0);
  Node* tree_1 = t1_t2.first;
  Node* tree_2 = t1_t2.second;
  tree->left = tree_2;
  if (tree_2) {
    tree_2->parent = tree;
  }
  FixNode(tree);
  if (tree_1) {
    tree_1->parent = nullptr;
  }
  FixNode(tree_1);
  return {tree_1, tree};
}
std::pair<Treap, Treap> Split(Treap tree, int64_t x0) {
  auto pair = Split(tree.root, x0);
  return {Treap(pair.first), Treap(pair.second)};
}

Node* Insert(Node* tree, int64_t idx, int64_t value) {
  auto trees = Split(tree, idx);
  return Merge(Merge(trees.first, new Node(value)), trees.second);
}
void Insert(Treap& tree, int64_t idx, int64_t value) {
  tree.root = Insert(tree.root, idx, value);
}

int64_t RMQ(Node* tree, int64_t left, int64_t right) {
  if (!tree) {
    return neutral;
  }
  if (right < 1 || tree->rank < left) {
    return neutral;
  }
  if (left <= 1 && tree->rank <= right) {
    return tree->result;
  }
  PushReverse(tree);
  int64_t k = (tree->left ? tree->left->rank : 0) + 1;
  int64_t result = ((left <= k && k <= right) ? tree->value : neutral);
  result = std::min(result, std::min(RMQ(tree->left, left, right), RMQ(tree->right, left - k, right - k)));
  return result;
}
int64_t RMQ(const Treap& tree, int64_t left, int64_t right) {
  return RMQ(tree.root, left, right);
}

void Reverse(Node* tree, int64_t left, int64_t right) {
  auto t2_t3 = Split(tree, right);
  auto t1_t2 = Split(t2_t3.first, left - 1);
  if (t1_t2.second) {
    t1_t2.second->reversed = !(t1_t2.second->reversed);
  }
  tree = Merge(Merge(t1_t2.first, t1_t2.second), t2_t3.second);
}
void Reverse(Treap& tree, int64_t left, int64_t right) {
  return Reverse(tree.root, left, right);
}

int main() {
  Treap tree;
  char query = '0';
  int64_t left_border = 0;
  int64_t right_border = 0;
  int64_t value = 0;
  int64_t n = 0;
  int64_t m = 0;
  std::cin >> n >> m;
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> value;
    Insert(tree, i, value);
  }
  for (int64_t i = 0; i < m; ++i) {
    std::cin >> query;
    if (query == '1') {
      std::cin >> left_border >> right_border;
      Reverse(tree, left_border, right_border);
    }
    if (query == '2') {
      std::cin >> left_border >> right_border;
      std::cout << RMQ(tree, left_border, right_border) << '\n';
    }
  }
}
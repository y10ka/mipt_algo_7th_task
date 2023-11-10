#include <iostream>
#include <vector>
#include <string>

int64_t none = 11111111111;

struct Node {
  int64_t key;
  int64_t value;
  int64_t rank;
  int64_t result;
  int64_t sub_min;
  int64_t sub_max;
  Node* left;
  Node* right;
  Node* parent;
  Node() = default;
  Node(int64_t key, int64_t value)
      : key(key)
      , value(value)
      , rank(1)
      , result(key)
      , sub_min(key)
      , sub_max(key)
      , left(nullptr)
      , right(nullptr)
      , parent(nullptr) {
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
  node->sub_min = (node->left ? node->left->sub_min : node->key);
  node->sub_max = (node->right ? node->right->sub_max : node->key);
  node->result = node->key + (node->left ? node->left->result : 0) + (node->right ? node->right->result : 0);
  node->rank = (node->left ? node->left->rank : 0) + (node->right ? node->right->rank : 0) + 1;
}

Node* Merge(Node* tree_1, Node* tree_2) {
  if (!tree_1) {
    return tree_2;
  }
  if (!tree_2) {
    return tree_1;
  }
  if (tree_2->value < tree_1->value) {
    tree_2->left = Merge(tree_1, tree_2->left);
    tree_2->left->parent = tree_2;
    FixNode(tree_2);
    return tree_2;
  }
  tree_1->right = Merge(tree_1->right, tree_2);
  tree_1->right->parent = tree_1;
  FixNode(tree_1);
  return tree_1;
}
Treap Merge(Treap& tree_1, Treap& tree_2) {
  return Treap(Merge(tree_1.root, tree_2.root));
}

std::pair<Node*, Node*> Split(Node* tree, int64_t x0) {
  if (!tree) {
    return {nullptr, nullptr};
  }
  if (tree->key < x0) {
    auto t1_t2 = Split(tree->right, x0);
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
std::pair<Treap, Treap> Split(Treap& tree, int64_t x0) {
  auto pair = Split(tree.root, x0);
  return {Treap(pair.first), Treap(pair.second)};
}

bool Exists(const Node* tree, int64_t key) {
  if (!tree) {
    return false;
  }
  if (tree->key == key) {
    return true;
  }
  return (tree->key > key ? Exists(tree->left, key) : Exists(tree->right, key));
}
bool Exists(const Treap& tree, int64_t key) {
  return Exists(tree.root, key);
}

Node* Insert(Node* tree, int64_t key, int64_t value) {
  if (Exists(tree, key)) {
    return tree;
  }
  auto trees = Split(tree, key);
  return Merge(Merge(trees.first, new Node(key, value)), trees.second);
}
void Insert(Treap& tree, int64_t key, int64_t value) {
  tree.root = Insert(tree.root, key, value);
}

int64_t RSQ(const Node* tree, int64_t left, int64_t right) {
  if (!tree) {
    return 0;
  }
  if (left <= tree->sub_min && right >= tree->sub_max) {
    return tree->result;
  }
  if (left > tree->sub_max || right < tree->sub_min) {
    return 0;
  }
  int64_t result = (left <= tree->key && tree->key <= right) * tree->key;
  result += RSQ(tree->left, left, right) + RSQ(tree->right, left, right);
  return result;
}
int64_t RSQ(const Treap& tree, int64_t left, int64_t right) {
  return RSQ(tree.root, left, right);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  Treap tree;
  char query = '0';
  int64_t left_border = 0;
  int64_t right_border = 0;
  int64_t value = 0;
  int64_t n = 0;
  std::cin >> n;
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> query;
    if (query == '+') {
      std::cin >> value;
      Insert(tree, value, rand());
    }
    if (query == '?') {
      std::cin >> left_border >> right_border;
      std::cout << RSQ(tree, left_border, right_border) << '\n';
    }
  }
}
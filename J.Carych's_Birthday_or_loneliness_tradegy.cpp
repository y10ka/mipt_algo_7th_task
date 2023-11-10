#include <iostream>
#include <vector>
#include <string>
#include <random>

int64_t none = -11111111111;

struct Node {
  int64_t key;
  int64_t value;
  int64_t rank;
  Node* left;
  Node* right;
  Node* parent;
  Node() = default;
  Node(int64_t key, int64_t value) : key(key), value(value), rank(1), left(nullptr), right(nullptr), parent(nullptr) {
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
};

void Delete(Treap& tree) {
  Delete(tree.root);
}

void FixNode(Node* node) {
  if (!node) {
    return;
  }
  node->rank = (node ? 1 : 0);
  node->rank += (node->left ? node->left->rank : 0);
  node->rank += (node->right ? node->right->rank : 0);
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
Treap Merge(Treap tree_1, Treap tree_2) {
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
std::pair<Treap, Treap> Split(Treap tree, int64_t x0) {
  auto pair = Split(tree.root, x0);
  return {Treap(pair.first), Treap(pair.second)};
}

bool Exists(Node* tree, int64_t key) {
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

int64_t Second(Node* tree) {
  if (!tree) {
    return 1000000;
  }
  while (tree->left) {
    tree = tree->left;
  }
  if (tree->parent) {
    return tree->parent->value;
  }
  if (tree->right) {
    tree = tree->right;
    while (tree->left) {
      tree = tree->left;
    }
    return tree->value;
  }
  return 1000000;
}

Node* Erase(Node* tree, int64_t key) {
  if (!Exists(tree, key)) {
    return tree;
  }
  auto trees = Split(tree, key);
  auto tree_1 = trees.first;
  auto tree_1_adv = trees.second;
  trees = Split(tree_1_adv, key + 1);
  delete trees.first;
  return Merge(tree_1, trees.second);
}
void Erase(Treap& tree, int64_t key) {
  tree.root = Erase(tree.root, key);
}

int64_t KthElement(Node* tree, int64_t idx) {
  if (tree->rank < idx + 1) {
    return none;
  }
  int64_t cur = 0;
  if (tree->left) {
    cur += tree->left->rank;
  }
  if (cur == idx) {
    return tree->key;
  }
  if (cur > idx) {
    return KthElement(tree->left, idx);
  }
  return KthElement(tree->right, idx - tree->left->rank - 1);
}
int64_t KthElement(const Treap& tree, int64_t idx) {
  return KthElement(tree.root, idx);
}

int64_t Next(Node* tree, int64_t x) {
  if (tree->key <= x) {
    if (!tree->right) {
      return none;
    }
    return Next(tree->right, x);
  }
  if (!tree->left) {
    return tree->key;
  }
  int64_t left_res = Next(tree->left, x);
  return (left_res == none ? tree->key : left_res);
}
int64_t Next(const Treap& tree, int64_t x) {
  return Next(tree.root, x);
}

int64_t Prev(Node* tree, int64_t x) {
  if (tree->key >= x) {
    if (!tree->left) {
      return none;
    }
    return Prev(tree->left, x);
  }
  if (!tree->right) {
    return tree->key;
  }
  int64_t right_res = Prev(tree->right, x);
  return (right_res == none ? tree->key : right_res);
}
int64_t Prev(const Treap& tree, int64_t x) {
  return Prev(tree.root, x);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  Treap tree;
  std::string query;
  int64_t x = 0;
  while (std::cin >> query >> x) {
    if (query == "insert") {
      Insert(tree, x, rand());
    }
    if (query == "delete") {
      Erase(tree, x);
    }
    if (query == "exists") {
      std::cout << (Exists(tree, x) ? "true" : "false") << '\n';
    }
    if (query == "next") {
      int64_t ans = Next(tree, x);
      std::cout << (ans == none ? "none" : std::to_string(ans)) << '\n';
    }
    if (query == "prev") {
      int64_t ans = Prev(tree, x);
      std::cout << (ans == none ? "none" : std::to_string(ans)) << '\n';
    }
    if (query == "kth") {
      int64_t ans = KthElement(tree, x);
      std::cout << (ans == none ? "none" : std::to_string(ans)) << '\n';
    }
  }
  Delete(tree);
}
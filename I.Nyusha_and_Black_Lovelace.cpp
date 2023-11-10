#include <iostream>
#include <vector>

struct KeyValue : public std::pair<int, int> {
  const int& Key() const {
    return first;
  }
  const int& Value() const {
    return second;
  }
  int& Key() {
    return first;
  }
  int& Value() {
    return second;
  }
};

using Map = std::vector<KeyValue>;
void Input(Map& data, int n) {
  for (int i = 0; i < n; ++i) {
    std::cin >> data[i].Key() >> data[i].Value();
    data[i].Key() = i + 1;
    ++data[i].Value();
  }
}

struct Node {
  int key;
  int value;
  Node* left;
  Node* right;
  Node* parent;
  Node() = default;
  Node(int key, int value) : key(key), value(value), left(nullptr), right(nullptr), parent(nullptr) {
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
  bool exist;
  Treap() : root(nullptr), exist(false) {
  }
  explicit Treap(Node* ptr) : root(ptr), exist(true) {
  }
  explicit Treap(const Map& data) : root(nullptr), exist(false) {
    Node* last = nullptr;
    Node* cur = nullptr;
    for (auto pair : data) {
      cur = last;
      while (cur != nullptr && cur->value > pair.Value()) {
        cur = cur->parent;
      }
      if (cur && cur->value == pair.Value()) {
        return;
      }
      auto new_node = new Node(pair.Key(), pair.Value());
      if (cur == nullptr) {
        if (root) {
          root->parent = new_node;
        }
        new_node->left = root;
        root = new_node;
      } else {
        new_node->left = cur->right;
        if (cur->right) {
          cur->right->parent = new_node;
        }
        cur->right = new_node;
        new_node->parent = cur;
      }
      last = new_node;
    }
    exist = true;
  }
  ~Treap() {
    Delete(root);
  }
};

void Print(Node* cur) {
  if (!cur) {
    return;
  }
  Print(cur->left);
  if (cur->parent) {
    std::cout << cur->parent->key << ' ';
  } else {
    std::cout << "0 ";
  }
  if (cur->left) {
    std::cout << cur->left->key << ' ';
  } else {
    std::cout << "0 ";
  }
  if (cur->right) {
    std::cout << cur->right->key << '\n';
  } else {
    std::cout << "0\n";
  }
  Print(cur->right);
}
void Print(const Treap& tree) {
  if (!tree.exist) {
    std::cout << "NO\n";
    return;
  }
  std::cout << "YES\n";
  Print(tree.root);
}

int main() {
  int n = 0;
  std::cin >> n;
  Map data(n);
  Input(data, n);
  Treap treap(data);
  Print(treap);
}
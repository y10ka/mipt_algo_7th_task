#include <iostream>
#include <vector>

using IntVector = std::vector<int64_t>;

struct Answer {
  int64_t result;
  int64_t left;
  int64_t right;
  Answer() : result(-1), left(-1), right(-1) {
  }
  Answer(int64_t res, int64_t l, int64_t r) : result(res), left(l), right(r) {
  }
  void Print() {
    std::cout << result << '\n';
    std::cout << left << ' ' << right << '\n';
  }
};

void Print(IntVector vec) {
  for (int i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << ' ';
  }
  std::cout << '\n';
}

Answer MaxSummary(const IntVector& array) {
  auto size = static_cast<int64_t>(array.size());
  IntVector prefix(size + 1, 0);
  auto iter = prefix.begin() + 1;
  for (auto value : array) {
    *iter = *(iter - 1) + value;
    ++iter;
  }
  IntVector prev(size + 1, 0);
  IntVector next(size + 1, 0);
  IntVector stack;
  for (int64_t i = 1; i < size + 1; ++i) {
    while (!stack.empty() && !(array[stack.back() - 1] < array[i - 1])) {
      stack.pop_back();
    }
    if (!stack.empty()) {
      prev[i] = stack.back();
    }
    stack.push_back(i);
  }
  stack.clear();
  for (int64_t i = size; i > 0; --i) {
    while (!stack.empty() && !(array[stack.back() - 1] < array[i - 1])) {
      stack.pop_back();
    }
    if (!stack.empty()) {
      next[i] = stack.back();
    }
    stack.push_back(i);
  }
  Answer answer(-1, 0, 0);
  int64_t cur = 0;
  for (int64_t i = 1; i < size + 1; ++i) {
    cur = array[i - 1] * (prefix[(next[i] ? next[i] - 1 : size)] - prefix[(prev[i] ? prev[i] : 0)]);
    if (answer.result == -1 || answer.result < cur) {
      answer.result = cur;
      answer.left = (prev[i] ? prev[i] + 1 : 1);
      answer.right = (next[i] ? next[i] - 1 : size);
    }
  }
  return answer;
}

int main() {
  int n = 0;
  std::cin >> n;
  IntVector array(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> array[i];
  }
  auto answer = MaxSummary(array);
  answer.Print();
}
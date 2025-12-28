#include "solution.hpp"

#include <algorithm>
#include <limits>
#include <vector>

namespace {

class SegTreeMin {
 public:
  explicit SegTreeMin(int n) : n_(n) {
    size_ = 1;
    while (size_ < n_) size_ <<= 1;
    tree_.assign(2 * size_, kInf);
  }

  void Build(const std::vector<int>& a) {
    for (int i = 0; i < n_; ++i) tree_[size_ + i] = a[i];
    for (int i = size_ - 1; i >= 1; --i) {
      tree_[i] = std::min(tree_[2 * i], tree_[2 * i + 1]);
    }
  }

  void Set(int idx, int value) {
    int v = size_ + idx;
    tree_[v] = value;
    v >>= 1;
    while (v >= 1) {
      tree_[v] = std::min(tree_[2 * v], tree_[2 * v + 1]);
      v >>= 1;
    }
  }

  int GetMin(int l, int r) const {
    l += size_;
    r += size_;
    int res = kInf;
    while (l <= r) {
      if (l & 1) res = std::min(res, tree_[l++]);
      if (!(r & 1)) res = std::min(res, tree_[r--]);
      l >>= 1;
      r >>= 1;
    }
    return res;
  }

 private:
  static constexpr int kInf = std::numeric_limits<int>::max();

  int n_ = 0;
  int size_ = 1;
  std::vector<int> tree_;
};

}  // namespace

void SolveTask06(std::istream& in, std::ostream& out) {
  int n = 0;
  int q = 0;
  if (!(in >> n >> q)) return;

  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) in >> a[i];

  SegTreeMin st(n);
  st.Build(a);

  for (int k = 0; k < q; ++k) {
    int type = 0;
    in >> type;
    if (type == 1) {
      int l = 0;
      int r = 0;
      in >> l >> r;
      --l;
      --r;
      out << st.GetMin(l, r) << "\n";
    } else if (type == 2) {
      int i = 0;
      int x = 0;
      in >> i >> x;
      --i;
      st.Set(i, x);
    }
  }
}

#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

namespace {

struct Edge {
  int u;
  int v;
  int w;
};

class Dsu {
 public:
  explicit Dsu(int n) : parent_(n), size_(n, 1) {
    std::iota(parent_.begin(), parent_.end(), 0);
  }

  int Find(int v) {
    while (parent_[v] != v) {
      parent_[v] = parent_[parent_[v]];
      v = parent_[v];
    }
    return v;
  }

  bool Unite(int a, int b) {
    a = Find(a);
    b = Find(b);
    if (a == b) return false;
    if (size_[a] < size_[b]) std::swap(a, b);
    parent_[b] = a;
    size_[a] += size_[b];
    return true;
  }

 private:
  std::vector<int> parent_;
  std::vector<int> size_;
};

constexpr const char* kImpossible =
    "Невозможно построить остовное дерево с заданным ограничением степени";

}  

void SolveTask05(std::istream& in, std::ostream& out) {
  int n = 0;
  int m = 0;
  int d = 0;
  if (!(in >> n >> m >> d)) return;

  std::vector<Edge> edges;
  edges.reserve(static_cast<size_t>(m));

  for (int i = 0; i < m; ++i) {
    int u = 0;
    int v = 0;
    int w = 0;
    in >> u >> v >> w;
    --u;
    --v;
    if (u < 0 || u >= n || v < 0 || v >= n) continue;
    if (u == v) continue;
    edges.push_back(Edge{u, v, w});
  }

  std::sort(edges.begin(), edges.end(),
            [](const Edge& a, const Edge& b) {
              if (a.w != b.w) return a.w < b.w;
              if (a.u != b.u) return a.u < b.u;
              return a.v < b.v;
            });

  if (n == 1) {
    out << 0 << "\n";
    return;
  }

  Dsu dsu(n);
  std::vector<int> deg(n, 0);

  long long total = 0;
  int used = 0;

  for (const auto& e : edges) {
    if (deg[e.u] >= d || deg[e.v] >= d) continue;
    if (dsu.Unite(e.u, e.v)) {
      ++deg[e.u];
      ++deg[e.v];
      total += e.w;
      ++used;
      if (used == n - 1) break;
    }
  }

  if (used != n - 1) {
    out << kImpossible << "\n";
    return;
  }

  out << total << "\n";
}

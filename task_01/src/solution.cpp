#include "solution.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace {

class Graph {
 public:
  explicit Graph(int vertices) : n_(vertices), g_(vertices) {}

  void AddEdge(int u, int v) {
    if (u < 0 || u >= n_ || v < 0 || v >= n_) return;
    if (u == v) return;
    int id = edge_id_++;
    g_[u].push_back(Edge{v, id});
    g_[v].push_back(Edge{u, id});
    computed_ = false;
  }

  std::vector<int> FindArticulationPoints() {
    Compute();
    std::vector<int> res;
    res.reserve(n_);
    for (int v = 0; v < n_; ++v) {
      if (is_articulation_[v]) res.push_back(v);
    }
    std::sort(res.begin(), res.end());
    return res;
  }

  std::vector<std::pair<int, int>> FindBridges() {
    Compute();
    return bridges_;
  }

 private:
  struct Edge {
    int to;
    int id;
  };

  void Compute() {
    if (computed_) return;
    computed_ = true;

    timer_ = 0;
    visited_.assign(n_, 0);
    tin_.assign(n_, -1);
    low_.assign(n_, -1);
    is_articulation_.assign(n_, 0);
    bridges_.clear();

    for (int v = 0; v < n_; ++v) {
      if (!visited_[v]) Dfs(v, -1);
    }

    for (auto& e : bridges_) {
      if (e.first > e.second) std::swap(e.first, e.second);
    }
    std::sort(bridges_.begin(), bridges_.end());
    bridges_.erase(std::unique(bridges_.begin(), bridges_.end()),
                   bridges_.end());
  }

  void Dfs(int v, int parent_edge_id) {
    visited_[v] = 1;
    tin_[v] = timer_;
    low_[v] = timer_;
    ++timer_;

    int children = 0;

    for (const Edge& e : g_[v]) {
      int to = e.to;
      if (e.id == parent_edge_id) continue;

      if (visited_[to]) {
        low_[v] = std::min(low_[v], tin_[to]);
        continue;
      }

      Dfs(to, e.id);
      low_[v] = std::min(low_[v], low_[to]);

      if (low_[to] > tin_[v]) {
        bridges_.push_back(std::make_pair(v, to));
      }

      if (parent_edge_id != -1 && low_[to] >= tin_[v]) {
        is_articulation_[v] = 1;
      }

      ++children;
    }

    if (parent_edge_id == -1 && children > 1) {
      is_articulation_[v] = 1;
    }
  }

  int n_ = 0;
  int edge_id_ = 0;
  int timer_ = 0;

  std::vector<std::vector<Edge>> g_;
  std::vector<int> visited_;
  std::vector<int> tin_;
  std::vector<int> low_;
  std::vector<int> is_articulation_;
  std::vector<std::pair<int, int>> bridges_;

  bool computed_ = false;
};

}  // namespace

void SolveTask01(std::istream& in, std::ostream& out) {
  int n = 0;
  int m = 0;
  if (!(in >> n >> m)) return;

  Graph g(n);

  for (int i = 0; i < m; ++i) {
    int u = 0;
    int v = 0;
    in >> u >> v;
    --u;
    --v;
    g.AddEdge(u, v);
  }

  std::vector<int> aps = g.FindArticulationPoints();
  std::vector<std::pair<int, int>> bridges = g.FindBridges();

  out << aps.size() << "\n";
  if (aps.empty()) {
    out << "-\n";
  } else {
    for (size_t i = 0; i < aps.size(); ++i) {
      if (i) out << " ";
      out << (aps[i] + 1);
    }
    out << "\n";
  }

  out << bridges.size() << "\n";
  if (bridges.empty()) {
    out << "-\n";
  } else {
    for (size_t i = 0; i < bridges.size(); ++i) {
      int a = bridges[i].first + 1;
      int b = bridges[i].second + 1;
      if (a > b) std::swap(a, b);
      out << a << " " << b;
      if (i + 1 != bridges.size()) out << "; ";
    }
    out << "\n";
  }
}

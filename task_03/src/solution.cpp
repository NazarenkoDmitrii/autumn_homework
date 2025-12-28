#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

namespace {

struct Edge {
  int from;
  int to;
  long long w;
};

struct AdjEdge {
  int to;
  long long w;
};

constexpr long long kInf = (std::numeric_limits<long long>::max() / 4);

bool BellmanFordPotentials(int n, const std::vector<Edge>& edges,
                           std::vector<long long>* h) {
  h->assign(n, 0LL);

  for (int i = 0; i < n; ++i) {
    bool relaxed = false;
    for (const auto& e : edges) {
      long long cand = (*h)[e.from] + e.w;
      if (cand < (*h)[e.to]) {
        (*h)[e.to] = cand;
        relaxed = true;
      }
    }
    if (!relaxed) break;
  }

  for (const auto& e : edges) {
    if ((*h)[e.from] + e.w < (*h)[e.to]) return false;
  }
  return true;
}

std::vector<long long> Dijkstra(int n, int s,
                                const std::vector<std::vector<AdjEdge>>& g) {
  std::vector<long long> dist(n, kInf);
  dist[s] = 0;

  using State = std::pair<long long, int>;
  std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
  pq.push({0, s});

  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (d != dist[v]) continue;

    for (const auto& e : g[v]) {
      if (dist[v] >= kInf) continue;
      long long nd = dist[v] + e.w;
      if (nd < dist[e.to]) {
        dist[e.to] = nd;
        pq.push({nd, e.to});
      }
    }
  }

  return dist;
}

}  // namespace

void SolveTask03(std::istream& in, std::ostream& out) {
  int n = 0;
  int m = 0;
  if (!(in >> n >> m)) return;

  std::vector<Edge> edges;
  edges.reserve(static_cast<size_t>(m));

  for (int i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    long long w = 0;
    in >> a >> b >> w;
    if (a < 0 || a >= n || b < 0 || b >= n) continue;
    edges.push_back(Edge{a, b, w});
  }

  std::vector<long long> h;
  if (!BellmanFordPotentials(n, edges, &h)) {
    out << "-1\n";
    return;
  }

  std::vector<std::vector<AdjEdge>> g_rw(n);
  for (const auto& e : edges) {
    long long w2 = e.w + h[e.from] - h[e.to];
    if (w2 < 0) w2 = 0;
    g_rw[e.from].push_back(AdjEdge{e.to, w2});
  }

  for (int s = 0; s < n; ++s) {
    std::vector<long long> d_rw = Dijkstra(n, s, g_rw);

    for (int v = 0; v < n; ++v) {
      if (v) out << " ";
      if (d_rw[v] >= kInf / 2) {
        out << "INF";
      } else {
        long long ans = d_rw[v] + h[v] - h[s];
        out << ans;
      }
    }
    out << "\n";
  }
}

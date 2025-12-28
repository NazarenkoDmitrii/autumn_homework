#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Edge {
  int to;
  long long w;
};

constexpr long long kInf = (std::numeric_limits<long long>::max() / 4);

std::vector<long long> Dijkstra(int n, int s, const std::vector<std::vector<Edge>>& g) {
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

} 

void SolveTask04(std::istream& in, std::ostream& out) {
  int n = 0;
  int m = 0;
  if (!(in >> n >> m)) return;

  std::vector<long long> rest;
  rest.reserve(static_cast<size_t>(3LL * m + 2));
  long long x = 0;
  while (in >> x) rest.push_back(x);

  int s = 0;
  size_t pos = 0;

  if (rest.size() == static_cast<size_t>(3LL * m)) {
    s = 0;
    pos = 0;
  } else if (rest.size() == static_cast<size_t>(3LL * m + 1)) {
    s = static_cast<int>(rest[0]);
    pos = 1;
  } else {
    out << "-1\n";
    return;
  }

  if (s < 0 || s >= n) {
    out << "-1\n";
    return;
  }

  std::vector<std::vector<Edge>> g(n);
  g.reserve(n);

  for (int i = 0; i < m; ++i) {
    int u = static_cast<int>(rest[pos + 3LL * i + 0]);
    int v = static_cast<int>(rest[pos + 3LL * i + 1]);
    long long w = rest[pos + 3LL * i + 2];
    if (u < 0 || u >= n || v < 0 || v >= n) continue;
    if (w < 0) continue;
    g[u].push_back(Edge{v, w});
  }

  std::vector<long long> dist = Dijkstra(n, s, g);

  for (int i = 0; i < n; ++i) {
    if (i) out << " ";
    if (dist[i] >= kInf / 2) {
      out << "INF";
    } else {
      out << dist[i];
    }
  }
  out << "\n";
}

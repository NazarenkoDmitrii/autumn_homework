#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <queue>
#include <utility>
#include <vector>

namespace {

void DfsOrderIter(int start, const std::vector<std::vector<int>>& g,
                  std::vector<int>* used, std::vector<int>* order) {
  std::vector<std::pair<int, int>> st;
  st.reserve(64);
  st.push_back({start, 0});
  while (!st.empty()) {
    int v = st.back().first;
    int& idx = st.back().second;
    if (idx == 0) (*used)[v] = 1;

    if (idx < static_cast<int>(g[v].size())) {
      int to = g[v][idx];
      ++idx;
      if (!(*used)[to]) st.push_back({to, 0});
    } else {
      order->push_back(v);
      st.pop_back();
    }
  }
}

void BfsAssign(int start, const std::vector<std::vector<int>>& gr,
               std::vector<int>* used, std::vector<int>* comp, int comp_id) {
  std::queue<int> q;
  q.push(start);
  (*used)[start] = 1;
  (*comp)[start] = comp_id;
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int to : gr[v]) {
      if (!(*used)[to]) {
        (*used)[to] = 1;
        (*comp)[to] = comp_id;
        q.push(to);
      }
    }
  }
}

int MinEdgesToMakeStronglyConnected(int n,
                                    const std::vector<std::pair<int, int>>& edges) {
  std::vector<std::vector<int>> g(n), gr(n);
  g.reserve(n);
  gr.reserve(n);

  for (const auto& e : edges) {
    int a = e.first;
    int b = e.second;
    if (a < 0 || a >= n || b < 0 || b >= n) continue;
    g[a].push_back(b);
    gr[b].push_back(a);
  }

  std::vector<int> used(n, 0);
  std::vector<int> order;
  order.reserve(n);

  for (int i = 0; i < n; ++i) {
    if (!used[i]) DfsOrderIter(i, g, &used, &order);
  }

  std::fill(used.begin(), used.end(), 0);
  std::vector<int> comp(n, -1);
  int comp_count = 0;

  for (int i = static_cast<int>(order.size()) - 1; i >= 0; --i) {
    int v = order[i];
    if (!used[v]) {
      BfsAssign(v, gr, &used, &comp, comp_count);
      ++comp_count;
    }
  }

  if (comp_count == 1) return 0;

  std::vector<int> has_in(comp_count, 0);
  std::vector<int> has_out(comp_count, 0);

  for (int v = 0; v < n; ++v) {
    int cv = comp[v];
    for (int to : g[v]) {
      int cu = comp[to];
      if (cv != cu) {
        has_out[cv] = 1;
        has_in[cu] = 1;
      }
    }
  }

  int sources = 0;
  int sinks = 0;
  for (int c = 0; c < comp_count; ++c) {
    if (!has_in[c]) ++sources;
    if (!has_out[c]) ++sinks;
  }

  return std::max(sources, sinks);
}

}  // namespace

void SolveTask02(std::istream& in, std::ostream& out) {
  int n = 0;
  int m = 0;
  if (!(in >> n >> m)) return;

  std::vector<std::pair<int, int>> edges;
  edges.reserve(static_cast<size_t>(m));

  for (int i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    in >> a >> b;
    edges.push_back({a, b});
  }

  out << MinEdgesToMakeStronglyConnected(n, edges) << "\n";
}

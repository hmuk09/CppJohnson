#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <iomanip>
#include <queue>

using namespace std;

typedef long long ll;

const int N = 200050;
const int K = 150050;
const int INF = 1e9 + 7;
const int maxCap = 1;

struct edge {
    int to, rev, cost, capacity, flow, num;

    edge()
    : to(0)
    , rev(0)
    , cost(0)
    , capacity(0)
    , flow(0)
    , num(0) {
    }

    edge(int to, int rev, int cost, int cap, int flow, int(num))
    : to(to)
    , rev(rev)
    , cost(cost)
    , capacity(cap)
    , flow(flow)
    , num(num) {
    }
};

int n, m, k;
vector<edge> edges[N];
bool visited[N];
pair<int, int> parent[N];
vector<int> dist;

void dfsWay(int v) {
    visited[v] = true;
    for (auto &edge : edges[v]) {
        if (!visited[edge.to] && edge.capacity > 0 && edge.flow == edge.capacity) {
            parent[edge.to] = make_pair(v, edge.rev);
            dfsWay(edge.to);
        }
    }
}

void rec(int v, int edges_num) {
    if (parent[v].first != -1) {
        rec(parent[v].first, edges_num + 1);
        edges[v][parent[v].second].flow -= 1;
        int ind_norm = edges[v][parent[v].second].rev;
        edges[parent[v].first][ind_norm].flow += 1;
        cout << edges[v][parent[v].second].num + 1 << " ";
    }
    else {
        cout << edges_num << " ";
    }
}

void findWay(int start, int final) {
    fill(parent, parent + n, make_pair(-1, -1));
    fill(visited, visited + n, false);
    dfsWay(start);
    rec(final, 0);
    cout << endl;
}

bool dijkstra(int start, int final) {
    vector<int> phi = dist;
    fill(dist.begin(), dist.end(), INF);
    fill(parent, parent + n, make_pair(-1, -1));
    dist[0] = 0;
    priority_queue<pair<int, int>> pq;
    pq.push(make_pair(0, start));
    while (!pq.empty()) {
        int cur_v = pq.top().second;
        int cur_distance = -pq.top().first;
        pq.pop();
        if (dist[cur_v] < cur_distance)
            continue;
        for (auto edge : edges[cur_v]) {
            if (edge.flow == edge.capacity)
                continue;
            int new_dist = dist[cur_v] + edge.cost + phi[cur_v] - phi[edge.to];
            if (new_dist < dist[edge.to]) {
                dist[edge.to] = new_dist;
                parent[edge.to] = {cur_v, edge.rev};
                pq.push(make_pair(-new_dist, edge.to));
            }
        }
    }
    return (dist[final] != INF);
}

int changeFlow(int v) {
    int cost = 0;
    while (parent[v].first != -1) {
        int p = parent[v].first;
        int ind_v_p = parent[v].second;
        int ind_p_v = edges[v][ind_v_p].rev;

        cost += edges[p][ind_p_v].cost;
        edges[p][ind_p_v].flow += 1;
        edges[v][ind_v_p].flow -= 1;

        v = parent[v].first;
    }

    return cost;
};

void add_edge(int from, int to, int cost, int capacity, int num) {
    edges[from].emplace_back(to, static_cast<int>(edges[to].size()), cost, capacity, 0, num);
    edges[to].emplace_back(from, static_cast<int>(edges[from].size() - 1), -cost, 0, 0, num);
}

int main() {
    cin >> n >> m >> k;

    dist.resize(n);

    for (int i = 0; i < m; ++i) {
        int x, y, cost;
        cin >> x >> y >> cost;
        --x;
        --y;
        add_edge(x, y, cost, maxCap, i);
        add_edge(y, x, cost, maxCap, i);
    }

    int start = 0, final = n - 1;
    int maxFlow = 0, sumCost = 0;
    int cost = 0;
    while (maxFlow < k && dijkstra(start, final)) {
        cost = changeFlow(final);
        maxFlow++;
        sumCost += cost;
    }
    if (maxFlow < k) {
        cout << -1;
    }
    else {
        cout << fixed << setprecision(5) << (double) sumCost / k << endl;
        for (int i = 0; i < maxFlow; ++i)
            findWay(start, final);
    }

    return 0;
}

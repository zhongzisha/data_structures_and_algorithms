#ifndef MAXFLOWMINCUT_H
#define MAXFLOWMINCUT_H

// 最大流最小割
/*
 * 1. Ford-Fulkerson 残差路径法
 * Edmonds-Karp是其一种实现，用BFS找增广路径
 *
 * 2. Push-Relabel (or preflow-push)
 * 进入一个顶点的流量可以大于等于流出的流量
 * 如果说Ford-Fulkerson是针对边操作，则Push-Relabel是针对顶点操作，类似对偶的感觉
 *
 * 3. Dinic算法
 *
 *
 * References:
 * 1. https://cp-algorithms.com/graph/push-relabel.html
 */

#include <vector>
#include <queue>
#include "MyCommon.h"
using std::vector;
using std::queue;
using std::pair;

namespace problems {

class MaxFlowMinCut {
public:
    MaxFlowMinCut() {

    }
    virtual ~MaxFlowMinCut() {

    }

    // capacity: 顶点对的容量, nxn
    // adj: 无向图的邻接表, nxm (m为顶点的neighbors个数)
    // s: 源, t: 汇
    static int BFS(vector<vector<int>>& capacity,
                   vector<vector<int>>& adj,
                   int s, int t, vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next]) {
                    parent[next] = cur;
                    int new_flow = std::min(flow, capacity[cur][next]);
                    if (next == t) {
                        return new_flow;
                    }
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    // n: 顶点个数
    static int MaxFlow_EdmondsKarp(vector<vector<int>>& capacity,
                                   vector<vector<int>>& adj,
                                   int s, int t, int n) {
        int flow = 0;
        vector<int> parent(n);
        int new_flow;
        while ((new_flow = BFS(capacity, adj, s, t, parent)) != 0) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
        }

        return flow;
    }

    static
    void Push(vector<vector<int>>& capacity,
              vector<int>& excess,
              vector<vector<int>>& flow,
              queue<int>& excess_vertices,
              int u, int v) {
        int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
        flow[u][v] += d;
        flow[v][u] -= d;
        excess[u] -= d;
        excess[v] += d;
        if (d && (excess[v] == d)) {
            excess_vertices.push(v);
        }
    }

    static
    void Relabel(vector<vector<int>>& capacity,
                 vector<vector<int>>& flow,
                 vector<int>& height,
                 int u, int n) {
        int d = 1e6;
        for (int i = 0; i < n; i++) {
            if ((capacity[u][i] - flow[u][i]) > 0) {
                d = std::min(d, height[i]);
            }
        }
        if (d < 1e6) {
            height[u] = d + 1;
        }
    }

    static
    void Discharge(vector<vector<int>>& capacity,
                   vector<int>& excess,
                   vector<vector<int>>& flow,
                   vector<int>& height,
                   vector<int>& seen,
                   queue<int>& excess_vertices,
                   int u, int n) {
        while (excess[u] > 0) {
            if (seen[u] < n) {
                int v = seen[u];
                if ((capacity[u][v] - flow[u][v]) > 0
                        && (height[u] > height[v])) {
                    Push(capacity, excess, flow, excess_vertices,
                         u, v);
                } else {
                    seen[u]++;
                }
            } else {
                Relabel(capacity, flow, height, u, n);
                seen[u] = 0;
            }
        }
    }

    static int MaxFlow_PushRelabel(vector<vector<int>>& capacity,
                                   vector<vector<int>>& adj,
                                   int s, int t, int n) {
        vector<int> height;height.assign(n,0);
        height[0] = n;
        vector<vector<int>> flow; flow.assign(n, vector<int>(n, 0));
        vector<int> excess; excess.assign(n, 0);
        excess[0] = 1e6;
        queue<int> excess_vertices;

        for (int i = 1; i < n; i++) {
            Push(capacity, excess, flow, excess_vertices, 0, i);
        }
        vector<int> seen; seen.assign(n, 0);

        while (!excess_vertices.empty()) {
            int u = excess_vertices.front();
            excess_vertices.pop();
            if (u != 0 && (u != (n-1))) {
                Discharge(capacity, excess, flow, height, seen, excess_vertices, u, n);
            }
        }

        for(int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << flow[i][j] << ", ";
            }
            std::cout << "\n";
        }

        int maxFlow = 0;
        for (int i = 0; i < n; i++) {
            maxFlow += flow[0][i];
        }
        return maxFlow;
    }


    static
    void PushFaster(int u, int v,
                    vector<int>& excess,
                    vector<vector<int>>& capacity,
                    vector<vector<int>>& flow) {
        int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
        flow[u][v] += d;
        flow[v][u] -= d;
        excess[u] -= d;
        excess[v] += d;
    }

    static
    vector<int> FindMaxHeightVertices(int s, int t, int n,
                                      vector<int>& excess,
                                      vector<int>& height) {
        vector<int> max_height;
        for (int i = 0; i < n; i++) {
            if (i != s && i != t && excess[i] > 0) {
                if (!max_height.empty() && height[i] > height[max_height[0]]) {
                    max_height.clear();
                }
                if (max_height.empty() || height[i] == height[max_height[0]]) {
                    max_height.push_back(i);
                }
            }
        }
        return max_height;
    }

    static int MaxFlow_PushRelabelFaster(vector<vector<int>>& capacity,
                                         vector<vector<int>>& adj,
                                         int s, int t, int n) {
        vector<int> height; height.assign(n, 0);
        height[s] = n;
        vector<vector<int>> flow; flow.assign(n, vector<int>(n, 0));
        vector<int> excess; excess.assign(n, 0);
        excess[0] = 1e6;
        for (int i = 0; i < n; i++) {
            if (i != s) {
                PushFaster(s, i, excess, capacity, flow);
            }
        }

        vector<int> current;
        while (!(current = FindMaxHeightVertices(s, t, n,
                                                 excess, height)).empty()) {
            for (int i : current) {
                bool pushed = false;
                for (int j = 0; j < n && excess[i]; j++) {
                    if (capacity[i][j] - flow[i][j] > 0
                            && height[i] == height[j] + 1) {
                        PushFaster(i, j,
                                   excess, capacity, flow);
                        pushed = true;
                    }
                }
                if (!pushed) {
                    Relabel(capacity, flow, height, i, n);
                    break;
                }
            }
        }

        for(int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << flow[i][j] << ", ";
            }
            std::cout << "\n";
        }

        int maxFlow = 0;
        for (int i = 0; i < n; i++) {
            maxFlow += flow[i][t];
        }
        return maxFlow;
    }

    typedef long long Long;
    struct FlowEdge {
        int v, u;
        Long cap, flow = 0;
        FlowEdge(int v, int u, Long cap) : v(v), u(u), cap(cap) {}
    };

    struct Dinic {
        const Long FLOW_INF = 1e18;
        vector<FlowEdge> edges;
        vector<vector<int>> adj;
        int n, m = 0;
        int s, t;
        vector<int> level, ptr;
        queue<int> q;

        Dinic(int n, int s, int t) : n(n), s(s), t(t) {
            adj.resize(n);
            level.resize(n);
            ptr.resize(n);
        }

        void AddEdge(int v, int u, Long cap) {
            edges.emplace_back(v, u, cap);
            edges.emplace_back(u, v, 0);
            adj[v].push_back(m);
            adj[u].push_back(m + 1);
            m += 2;
        }

        bool BFS() {
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (int id : adj[v]) {
                    if (edges[id].cap - edges[id].flow < 1) {
                        continue;
                    }
                    if (level[edges[id].u] != -1) {
                        continue;
                    }
                    level[edges[id].u] = level[v] + 1;
                    q.push(edges[id].u);
                }
            }
            return level[t] != -1;
        }

        Long DFS(int v, Long pushed) {
            if (pushed == 0) {
                return 0;
            }
            if (v == t) {
                return pushed;
            }
            for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
                int id = adj[v][cid];
                int u = edges[id].u;
                if (level[v] + 1 != level[u] ||
                        edges[id].cap - edges[id].flow < 1) {
                    continue;
                }

                Long tr = DFS(u, std::min(pushed, edges[id].cap - edges[id].flow));
                if (tr == 0)
                    continue;
                edges[id].flow += tr;
                edges[id ^ 1].flow -= tr;
                return tr;
            }
            return 0;
        }

        Long MaxFlow_Dinic() {
            Long f = 0;
            while (true) {
                std::fill(level.begin(), level.end(), -1);
                level[s] = 0;
                q.push(s);
                if (!BFS()) {
                    break;
                }
                std::fill(ptr.begin(), ptr.end(), 0);
                while (Long pushed = DFS(s, FLOW_INF)) {
                    f += pushed;
                }
            }
            return f;
        }
    };

    // 最小代价最大流（或者最小耗费最大流）
    // 在所有最大流中找出最小代价的那个最大流
    struct MinCostFlow {
        const int INF = 1e9;
        int N;
        vector<vector<int>> adj;
        vector<vector<int>> cost;
        vector<vector<int>> capacity;

        MinCostFlow(int n,
                       vector<vector<int>>&& adj,
                       vector<vector<int>>&& cost,
                       vector<vector<int>>&& capacity)
            : N(n), adj(adj), cost(cost), capacity(capacity) {
        }

        // Bellman-Ford找单源最短路径
        void ShortestPaths(int v0,
                           vector<int>& d,
                           vector<int>& p) {
            d.assign(N, INF);
            d[v0] = 0;
            vector<bool> inq(N, false);
            queue<int> q;
            q.push(v0);
            p.assign(N, -1);

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = false;
                for (int v : adj[u]) {//对于每个边
                    if (capacity[u][v] > 0 &&
                            d[v] > d[u] + cost[u][v]) {
                        d[v] = d[u] + cost[u][v];//更新从源点到该点的最小代价
                        p[v] = u;//保存该点的上一个顶点
                        if (!inq[v]) {//如果该顶点不在队列中，加入队列
                            inq[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
            //循环结束时，d中保存v0到每个顶点的最小代价
            //p中保存最短路径上的前一个顶点，可用于后向追踪最短路径
        }

        //找流为K的最小代价流
        //如果K为无穷大，则是求最小代价最大流
        int Run(int K, int s, int t) {

            int maxFlow = 0;
            int minCost = 0;
            vector<int> d, p;
            while (maxFlow < K) {

                //在残差或者剩余网络中求起点的最小代价路径
                ShortestPaths(s, d, p);
                if (d[t] == INF) { // 如果起点到终点没有最小代价路径，则返回
                    break;
                }

                //从最小代价路径上向前追踪每条边
                //找到最小代价路径上的最大流
                int f = K - maxFlow;
                int cur = t;
                while (cur != s) {
                    f = std::min(f, capacity[p[cur]][cur]);
                    cur = p[cur];
                }

                //更新边的容量，残差网络
                maxFlow += f;
                minCost += f * d[t];
                cur = t;
                while (cur != s) {
                    capacity[p[cur]][cur] -= f;
                    capacity[cur][p[cur]] += f;
                    cur = p[cur];
                }
            }

            std::cout <<"flow=" <<maxFlow <<"\n";
            std::cout <<"cost=" <<minCost <<"\n";

            if (maxFlow < K) {
                return -1;
            } else {
                return minCost;
            }
        }

    };

};

}

#endif // MAXFLOWMINCUT_H

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
 * https://cp-algorithms.com/graph/push-relabel.html
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

};

}

#endif // MAXFLOWMINCUT_H

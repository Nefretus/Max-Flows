#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>

class EdmondKarp {
public:
    EdmondKarp(const std::string& filepath) {
        std::fstream file(filepath, std::ios_base::in);
        int n_nodes;
        file >> n_nodes;
        graph = std::vector<std::vector<int>>(n_nodes, std::vector<int>(n_nodes, 0));
        for (int i = 0; i < n_nodes; i++) {
            for (int j = 0; j < n_nodes; j++)
                file >> graph[i][j];
        }
    }

    void run() {
        int source = 0, target = graph.size() - 1, max_flow = 0;
        std::vector<std::vector<int>> residual_graph(graph);
        std::vector<int> parent(graph.size(), -1);
        while (bfs(residual_graph, source, target, parent)) {
            int i, j, flow = INT_MAX;
            for (i = target; i != source; i = parent[i]) {
                j = parent[i];
                flow = std::min(flow, residual_graph[j][i]);
            }
            for (i = target; i != source; i = parent[i]) {
                j = parent[i];
                residual_graph[i][j] += flow;
                residual_graph[j][i] -= flow;
            }
            max_flow += flow;
        }
        std::cout << max_flow << std::endl;
    }
private:
    bool bfs(std::vector<std::vector<int>>& residual_graph, int source, int target, std::vector<int>& path) {
        std::queue<int> queue;
        std::vector<bool> visited(residual_graph.size(), false);
        queue.push(source);
        visited[source] = true;
        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();
            for (int neighbor = 0; neighbor < residual_graph.size(); neighbor++) {
                if (residual_graph[vertex][neighbor] > 0 && !visited[neighbor]) {
                    path[neighbor] = vertex;
                    if (neighbor == target)
                        return true;
                    queue.push(neighbor);
                    visited[vertex] = true;
                }
            }
        }
        return false;
    }
    std::vector<std::vector<int>> graph;
};

class DinicMethod {
public:
    DinicMethod(const std::string& filepath) : source(0) {
        std::fstream file(filepath, std::ios_base::in);
        int n_nodes;
        file >> n_nodes;
        graph.resize(n_nodes);
        for (int i = 0; i < n_nodes; i++) {
            for (int j = 0; j < n_nodes; j++) {
                int capacity;
                file >> capacity;
                if (capacity) {
                    graph[i].push_back(Edge{ j, 0, capacity, static_cast<int>(graph[j].size()) });
                    graph[j].push_back(Edge{ i, 0, 0, static_cast<int>(graph[i].size()) });
                }
            }
        }
        target = n_nodes - 1;
    }

    void run() {
        int total_flow = 0;
        while (bfs()) {
            std::vector<int> n_edges_visited(graph.size() + 1, 0);
            while (int flow = dfs(source, target, INT_MAX, n_edges_visited))
                total_flow += flow;
        }
        std::cout << total_flow;
    }
private:
    bool bfs() {
        std::queue<int> queue;
        levels = std::vector<int>(graph.size(), -1);
        queue.push(source);
        levels[source] = 0;
        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();
            for (auto& edge : graph[vertex]) {
                if (levels[edge.target] < 0 && edge.flow < edge.capacity) {
                    queue.push(edge.target);
                    levels[edge.target] = levels[vertex] + 1;
                }
            }
        }
        return levels[target] >= 0;
    }

    int dfs(int from, int to, int flow, std::vector<int>& n_edges_visited) {
        if (from == to)
            return flow;
        for (; n_edges_visited[from] < graph[from].size(); n_edges_visited[from]++) {
            Edge& edge = graph[from][n_edges_visited[from]];
            if (levels[edge.target] == levels[from] + 1 && edge.flow < edge.capacity) {
                int min_flow = std::min(flow, edge.capacity - edge.flow);
                int temp = dfs(edge.target, to, min_flow, n_edges_visited);
                if (temp > 0) {
                    edge.flow += temp;
                    graph[edge.target][edge.rev].flow -= temp;
                    return temp;
                }
            }
        }
        return 0;
    }

    struct Edge {
        int target, flow, capacity, rev;
    };

    std::vector<std::vector<Edge>> graph;
    std::vector<int> levels;
    int source, target;
};

int main(int argc, char** argv) {
    EdmondKarp("data4.txt").run();
    DinicMethod("data4.txt").run();
    return 0;
}


#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>

std::vector<std::vector<int>> load_graph() {
    std::fstream file("data3.txt", std::ios_base::in);
    int n_nodes;
    file >> n_nodes;
    std::vector<std::vector<int>> matrix(n_nodes, std::vector<int>(n_nodes, 0));
    for (int i = 0; i < n_nodes; i++) {
        for (int j = 0; j < n_nodes; j++)
            file >> matrix[i][j];
    }
    return matrix;
}

bool find_path(std::vector<std::vector<int>>& residual_graph, int source, int target, std::vector<int>& path) {
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

void ford_fulkerson() {
    auto graph = load_graph();
    int source = 0, target = graph.size() - 1, max_flow = 0;
    std::vector<std::vector<int>> residual_graph(graph);
    std::vector<int> parent(graph.size(), -1);
    while (find_path(residual_graph, source, target, parent)) {
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

int main(int argc, char** argv) {
    ford_fulkerson();
    return 0;
}


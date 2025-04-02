#include "graph_Animation.h"


DijkstraVisualizer::DijkstraVisualizer(Graph* _graph) {
    graph = _graph;

    if (graph != nullptr) {
        distances = vector<int>(graph -> Nodes.size() + 1, INT_MAX);
        processed = vector<bool>(graph -> Nodes.size() + 1, false);
        if (graph -> dijkstraSource != -1) {
            distances[graph -> dijkstraSource] = 0;
        }
    }
    else {
        distances = vector<int>(0);
        processed = vector<bool>(0);
    }
    
    first_step = true;
    current_u = -1;
    current_neighbor_index = 0;
}

DijkstraStepResult DijkstraVisualizer::step() {
    if (first_step) {
        first_step = false;
        return {DijkstraStepResult::INIT};
    }
    
    bool all_processed = true;
    for (int vertex = 1; vertex <= graph -> numNode; ++vertex) {
        if (!processed[vertex]) {
            all_processed = false;
            break;
        }
    }
    if (all_processed) {
        return {DijkstraStepResult::DONE};
    }

    if (current_u != -1) {
        while(current_neighbor_index < (graph -> Adjacent_list[current_u].size())
            && processed[graph->Edges[graph->Adjacent_list[current_u][current_neighbor_index]].other(current_u)]) {
            current_neighbor_index++;
        }
    }

    if (current_u == -1 || current_neighbor_index >= graph -> Adjacent_list[current_u].size()) {
        int u = selectNextU();
        if (u == -1) return {DijkstraStepResult::DONE};
        current_u = u;
        current_neighbor_index = 0;
        processed[u] = true;
        return {DijkstraStepResult::SELECT_U, u};
    }
    else {
        int edge_index = graph->Adjacent_list[current_u][current_neighbor_index];
        int v = graph->Edges[edge_index].other(current_u);
        int alt = distances[current_u] + graph->Edges[edge_index].w; 
        
        if (alt < distances[v]) {
            distances[v] = alt;
        }

        current_neighbor_index++;
        return {DijkstraStepResult::RELAX_EDGE, current_u, v};
    }
}

int DijkstraVisualizer::selectNextU() {
    int min_dist = INT_MAX;
    int next_u = -1;
    for (int i = 1; i <= graph -> numNode; ++i) {
        if (!processed[i] && distances[i] < min_dist) {
            min_dist = distances[i];
            next_u = i;
        }
    }
    return next_u;
}
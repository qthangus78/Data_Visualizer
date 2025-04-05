#include "graph.h"

void GraphVisualizer::DrawDIJKSTRA() const {
    std::vector<int> DIJKSTRA_edges = GraphAlgorithms::getDIJKSTRA(graph, graph->dijkstraSource);
    for (int i = 0; i < graph->Edges.size(); ++i) {
        int special = 0;
        for (int id : DIJKSTRA_edges) {
            if (i == id) {
                special = 1;  // Part of Dijkstra tree
                break;
            }
        }
        DrawEdge(graph->Edges[i], special);
    }
    for (int u = 1; u <= graph->numNode; ++u) {
        DrawNode(u);
    }
}

void GraphVisualizer::DrawDIJKSTRA_StepByStep() {
    if (!graph || graph->dijkstraSource == -1 || !dijkstra_data.inited) {
        return;
    }

    // Handle step interval
    if (!dijkstra_data.is_paused) {
        dijkstra_data.elapsed_time += GetFrameTime();
        if (dijkstra_data.elapsed_time >= dijkstra_data.step_interval) {
            if (dijkstra_data.first_step) {
                dijkstra_data.first_step = false;
                dijkstra_data.step_result = {DijkstraStepResult::INIT};
            } else {
                GetDijkstraStep();
            }
            dijkstra_data.elapsed_time -= dijkstra_data.step_interval;
        }
    }

    if (dijkstra_data.step_result.action == DijkstraStepResult::DONE) {
        cout << "Dijkstra's algorithm completed." << endl;
    }

    for (int i = 0; i < graph->Edges.size(); ++i) {
        int special = 0;
        // Edge being currently relaxed
        if (dijkstra_data.step_result.action == DijkstraStepResult::RELAX_EDGE && 
            ((dijkstra_data.step_result.u == graph->Edges[i].u && dijkstra_data.step_result.v == graph->Edges[i].v) 
        ||   (dijkstra_data.step_result.u == graph->Edges[i].v && dijkstra_data.step_result.v == graph->Edges[i].u))) {
            special = 2;  // Currently being visited
        }
        // Edge in Dijkstra tree
        else if (dijkstra_data.processed[graph->Edges[i].u] && 
                 dijkstra_data.distances[graph->Edges[i].v] == 
                 dijkstra_data.distances[graph->Edges[i].u] + graph->Edges[i].w) {
            special = 1;  // Part of shortest path tree
        }
        else if (dijkstra_data.processed[graph->Edges[i].v] && 
                 dijkstra_data.distances[graph->Edges[i].u] == 
                 dijkstra_data.distances[graph->Edges[i].v] + graph->Edges[i].w) {
            special = 1;  // Part of shortest path tree
        }
        DrawEdge(graph->Edges[i], special);
    }
    for (int u = 1; u <= graph->numNode; ++u) {
        bool special = 1;
        if (u != dijkstra_data.current_u || dijkstra_data.step_result.action == DijkstraStepResult::DONE)   
            special = 0;
        DrawNode(u, special);
    }
}

GraphVisualizer::DijkstraVisualizerData::DijkstraVisualizerData() {
    codes = {
        "for each vertex v in Graph:",
        "    distance[v] := infinity",
        "    previous[v] := undefined",
        "distance[source] := 0",
        "Q := the set of all vertices in Graph",
        "while Q is not empty:",
        "    u := vertex in Q with smallest distance[]",
        "    remove u from Q",
        "    for each neighbor v of u:",
        "        alt := distance[u] + length(u, v)",
        "        if alt < distance[v]:",
        "            distance[v] := alt",
        "            previous[v] := u"
    };
}

void GraphVisualizer::initDijkstra() {
    if (graph != nullptr) {
        dijkstra_data.distances = std::vector<int>(graph->Nodes.size() + 1, INT_MAX);
        dijkstra_data.processed = std::vector<bool>(graph->Nodes.size() + 1, false);
        if (graph->dijkstraSource != -1) {
            dijkstra_data.distances[graph->dijkstraSource] = 0;
        }
    }
    dijkstra_data.first_step = true;
    dijkstra_data.current_u = -1;
    dijkstra_data.current_neighbor_index = 0;
    dijkstra_data.elapsed_time = 0.0f;
    dijkstra_data.is_paused = false;
    dijkstra_data.inited = true;
}

int GraphVisualizer::selectNextDijkstraU() {
    if (!graph || !dijkstra_data.inited) return -1;
    int min_dist = INT_MAX;
    int next_u = -1;
    for (int i = 1; i <= graph -> numNode; ++i) {
        if (!dijkstra_data.processed[i] && dijkstra_data.distances[i] < min_dist) {
            min_dist = dijkstra_data.distances[i];
            next_u = i;
        }
    }
    return next_u;
}

void GraphVisualizer::GetDijkstraStep() {
    if (!graph || !dijkstra_data.inited) return;
    bool all_processed = true;
    for (int vertex = 1; vertex <= graph->numNode; ++vertex) {
        if (!dijkstra_data.processed[vertex]) {
            all_processed = false;
            break;
        }
    }
    
    if (all_processed) {
        dijkstra_data.step_result = {DijkstraStepResult::DONE};
        return;
    }

    if (dijkstra_data.current_u != -1) {
        while (dijkstra_data.current_neighbor_index < graph->Adjacent_list[dijkstra_data.current_u].size() 
               && dijkstra_data.processed[graph->Edges[graph->Adjacent_list[dijkstra_data.current_u][dijkstra_data.current_neighbor_index]].other(dijkstra_data.current_u)]) {
            dijkstra_data.current_neighbor_index++;
        }
    }

    if (dijkstra_data.current_u == -1 || dijkstra_data.current_neighbor_index >= graph->Adjacent_list[dijkstra_data.current_u].size()) {
        int u = selectNextDijkstraU();
        if (u == -1) {
            dijkstra_data.step_result = {DijkstraStepResult::DONE};
            return;
        }
        dijkstra_data.current_u = u;
        dijkstra_data.current_neighbor_index = 0;
        dijkstra_data.processed[u] = true;
        dijkstra_data.step_result = {DijkstraStepResult::SELECT_U, u};
    } else {
        int edge_index = graph->Adjacent_list[dijkstra_data.current_u][dijkstra_data.current_neighbor_index];
        int v = graph->Edges[edge_index].other(dijkstra_data.current_u);
        int alt = dijkstra_data.distances[dijkstra_data.current_u] + graph->Edges[edge_index].w;

        if (alt < dijkstra_data.distances[v]) {
            dijkstra_data.distances[v] = alt;
        }

        dijkstra_data.current_neighbor_index++;
        dijkstra_data.step_result = {DijkstraStepResult::RELAX_EDGE, dijkstra_data.current_u, v};
    }
}
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
    
    // Draw pseudocode announcement box
    DrawDijkstraPseudoCode();
}

void GraphVisualizer::DrawDijkstraPseudoCode() {
    // Determine which lines to highlight
    int highlightStart = -1;
    int highlightEnd = -1;
    
    switch (dijkstra_data.step_result.action) {
        case DijkstraStepResult::INIT:
            highlightStart = 0;
            highlightEnd = 4;
            break;
        case DijkstraStepResult::SELECT_U:
            highlightStart = 6;
            highlightEnd = 7;
            break;
        case DijkstraStepResult::RELAX_EDGE:
            highlightStart = 8;
            highlightEnd = 12;
            break;
        case DijkstraStepResult::DONE:
            highlightStart = -1;
            highlightEnd = -1;
            break;
    }
    
    // Set highlighted lines
    algorithmBox.SetHighlightLines(highlightStart, highlightEnd);
    
    // Update info lines
    algorithmBox.ClearInfoLines();
    
    if (graph->dijkstraSource != -1) {
        char buffer[20];
        sprintf(buffer, "%d", graph->dijkstraSource);
        algorithmBox.AddInfoLine("Source", buffer);
    }
    
    if (dijkstra_data.current_u != -1) {
        char buffer[20];
        sprintf(buffer, "%d", dijkstra_data.current_u);
        algorithmBox.AddInfoLine("Current u", buffer);
    }
    
    if (dijkstra_data.step_result.action == DijkstraStepResult::RELAX_EDGE) {
        char buffer[30];
        sprintf(buffer, "%d -> %d", dijkstra_data.step_result.u, dijkstra_data.step_result.v);
        algorithmBox.AddInfoLine("Relaxing edge", buffer);
    }
    
    // Draw the announcement box
    algorithmBox.Draw();
}

GraphVisualizer::DijkstraVisualizerData::DijkstraVisualizerData() {
    codes = {
        "for v in Graph:",
        "    d[v] = infinity",
        "    prev[v] = nil",
        "d[src] = 0",
        "Q = all vertices",
        "while Q not empty:",
        "    u = min_dist vertex in Q",
        "    remove u from Q",
        "    for v adjacent to u:",
        "        alt = d[u] + len(u,v)",
        "        if alt < d[v]:",
        "            d[v] = alt",
        "            prev[v] = u"
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
    
    // Setup announcement box
    float boxMargin = 20.0f;
    Rectangle codeBox = {
        GraphDisplayScreen.x + GraphDisplayScreen.width + boxMargin,
        GraphDisplayScreen.y,
        300,
        GraphDisplayScreen.height
    };
    algorithmBox = AnnouncementBox(codeBox, "Dijkstra's Algorithm");
    algorithmBox.SetContent(dijkstra_data.codes);
    
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
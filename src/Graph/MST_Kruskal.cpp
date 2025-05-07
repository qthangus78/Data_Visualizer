#include "graph.h"
#include <algorithm>
#include <vector>

bool GraphVisualizer::isKruskalInited() const { 
    return kruskal_data.inited; 
}

KruskalVisualizerData::KruskalVisualizerData() {
    codes = {
        "Sort edges by weight",
        "Initialize disjoint sets",
        "for each edge (u,v) in sorted edges:",
        "    if find(u) != find(v):",
        "        add edge to MST",
        "        union(u,v)"
    };
}

void GraphVisualizer::initKruskal() {
    if (!graph) return;
    
    kruskal_data.parent.resize(graph->numNode + 1);
    kruskal_data.in_mst.clear();
    kruskal_data.in_mst.resize(graph->Edges.size(), false);
    
    // Initialize parent array for disjoint sets
    for (int i = 0; i <= graph->numNode; i++) {
        kruskal_data.parent[i] = -1;
    }
    
    // Prepare sorted edges
    kruskal_data.edges.clear();
    for (int i = 0; i < graph->Edges.size(); i++) {
        kruskal_data.edges.push_back({graph->Edges[i].w, i});
    }
    sort(kruskal_data.edges.begin(), kruskal_data.edges.end());
    
    // Setup announcement box
    float boxMargin = 20.0f;
    Rectangle codeBox = {
        GraphDisplayScreen.x + GraphDisplayScreen.width + boxMargin,
        GraphDisplayScreen.y,
        300,
        GraphDisplayScreen.height
    };
    algorithmBox = AnnouncementBox(codeBox, "Kruskal's MST Algorithm");
    algorithmBox.SetContent(kruskal_data.codes);
    
    // Clear history
    kruskal_data.history.clear();
    
    kruskal_data.current_edge_index = -1;
    kruskal_data.first_step = true;
    kruskal_data.elapsed_time = 0.0f;
    is_paused = true;
    kruskal_data.inited = true;
}

int GraphVisualizer::findSet(int v) {
    if (kruskal_data.parent[v] < 0) return v;
    return kruskal_data.parent[v] = findSet(kruskal_data.parent[v]);
}

bool GraphVisualizer::unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a == b) return false;
    
    if (kruskal_data.parent[a] > kruskal_data.parent[b]) 
        swap(a, b);
    kruskal_data.parent[a] += kruskal_data.parent[b];
    kruskal_data.parent[b] = a;
    return true;
}

void GraphVisualizer::GetKruskalStep() {
    if (!graph || !kruskal_data.inited) return;
    
    kruskal_data.history.push_back(KruskalState(
        kruskal_data.parent,
        kruskal_data.in_mst,
        kruskal_data.current_edge_index,
        kruskal_data.first_step
    ));

    if (++kruskal_data.current_edge_index >= (int)kruskal_data.edges.size()) {
        return;  // Algorithm complete
    }
    
    int edge_idx = kruskal_data.edges[kruskal_data.current_edge_index].second;
    int u = graph->Edges[edge_idx].u;
    int v = graph->Edges[edge_idx].v;
    
    if (unionSets(u, v)) {
        kruskal_data.in_mst[edge_idx] = true;
    }
}

void GraphVisualizer::DrawMST_StepByStep() {
    if (!graph || !kruskal_data.inited) return;

    if (!is_paused) {
        kruskal_data.elapsed_time += GetFrameTime() * GetSpeed();
        if (kruskal_data.elapsed_time >= kruskal_data.step_interval) {
            if (kruskal_data.first_step) {
                kruskal_data.first_step = false;
                
                // Save initial state to history
                kruskal_data.history.push_back(KruskalState(
                    kruskal_data.parent,
                    kruskal_data.in_mst,
                    kruskal_data.current_edge_index,
                    kruskal_data.first_step
                ));
            } else {
                GetKruskalStep();
            }
            kruskal_data.elapsed_time -= kruskal_data.step_interval;
        }
    }

    if (is_paused) {
        if (UndoButton.isPressed()) {
            UndoKruskal();
        }
        else if (RedoButton.isPressed()) {
            RedoKruskal();
        }
    }

     // Draw edges
     for (int i = 0; i < graph->Edges.size(); ++i) {
        int special = 0;
        // Currently being considered edge
        if (kruskal_data.current_edge_index >= 0 && kruskal_data.current_edge_index < kruskal_data.edges.size() &&
            i == kruskal_data.edges[kruskal_data.current_edge_index].second) {
            special = 2;  // Red color
        }
        // Edge already in MST
        else if (kruskal_data.in_mst[i]) {
            special = 1;  // Blue color
        }
        DrawEdge(graph->Edges[i], special);
    }

    // Draw nodes
    for (int u = 1; u <= graph->numNode; ++u) {
        DrawNode(u);
    }
    
    // Draw pseudocode announcement box
    DrawKruskalPseudoCode();
    
    // Update info lines
    UpdateKruskalInfoLines();
    
    // Draw the announcement box
    algorithmBox.Draw();
}

void GraphVisualizer::DrawKruskalPseudoCode() {
    // Determine which lines to highlight based on current state
    int highlightStart = -1;
    int highlightEnd = -1;
    
    if (kruskal_data.first_step) {
        highlightStart = 0;
        highlightEnd = 1;
    } else if (kruskal_data.current_edge_index >= 0 && 
               kruskal_data.current_edge_index < (int)kruskal_data.edges.size()) {
        
        int edge_idx = kruskal_data.edges[kruskal_data.current_edge_index].second;
        int u = graph->Edges[edge_idx].u;
        int v = graph->Edges[edge_idx].v;
        
        highlightStart = 3;
        highlightEnd = 5;
    } else {
        // Algorithm complete
        highlightStart = -1;
        highlightEnd = -1;
    }
    
    // Set highlighted lines
    algorithmBox.SetHighlightLines(highlightStart, highlightEnd);
}

bool GraphVisualizer::UndoKruskal() {
    // Check if there's any history to go back to
    if (kruskal_data.history.empty()) {
        return false;  // Can't undo further
    }
    
    // Get the previous state
    KruskalState prevState = kruskal_data.history.back();
    kruskal_data.history.pop_back();
    
    // Restore algorithm state
    kruskal_data.parent = prevState.parent;
    kruskal_data.in_mst = prevState.in_mst;
    kruskal_data.current_edge_index = prevState.current_edge_index;
    kruskal_data.first_step = prevState.first_step;
    
    return true;
}

bool GraphVisualizer::RedoKruskal() {
    if (kruskal_data.current_edge_index >= (int)kruskal_data.edges.size()) {
        return false;  // Algorithm already complete
    }
    
    GetKruskalStep();
    return true;
}

void GraphVisualizer::UpdateKruskalTable() {
    // Start a new table
    algorithmBox.StartTable();
    
    // Set the header with column titles
    std::vector<std::string> header = {"Edge", "Weight", "in MST"};
    algorithmBox.SetTableHeader(header);
    
    // Add table data rows for all edges in sorted order
    for (int i = 0; i < kruskal_data.edges.size(); i++) {
        std::vector<std::string> row(3);
        int edge_idx = kruskal_data.edges[i].second;
        int u = graph->Edges[edge_idx].u;
        int v = graph->Edges[edge_idx].v;
        
        // Column 1: Edge vertices
        row[0] = std::to_string(u) + " - " + std::to_string(v);
        
        // Column 2: Weight
        row[1] = std::to_string(graph->Edges[edge_idx].w);
        
        // Column 3: Included in MST?
        row[2] = kruskal_data.in_mst[edge_idx] ? "Yes" : "No";
        
        // Highlight the current edge being considered
        bool highlightRow = (i == kruskal_data.current_edge_index);
        
        algorithmBox.AddTableRow(row, highlightRow);
    }
    
    // End the table
    algorithmBox.EndTable();
}

void GraphVisualizer::UpdateKruskalInfoLines() {
    algorithmBox.ClearInfoLines();

    // First add the table
    UpdateKruskalTable();
    
    // Add a separator after the table
    algorithmBox.AddInfoLine("__SEPARATOR__", "");
    
    // Add MST weight
    int mstWeight = 0;
    for (int i = 0; i < graph->Edges.size(); i++) {
        if (kruskal_data.in_mst[i]) {
            mstWeight += graph->Edges[i].w;
        }
    }
    algorithmBox.AddInfoLine("MST Weight", std::to_string(mstWeight));
    
    // Add current edge being considered
    if (kruskal_data.current_edge_index >= 0 && 
        kruskal_data.current_edge_index < (int)kruskal_data.edges.size()) {
        
        int edge_idx = kruskal_data.edges[kruskal_data.current_edge_index].second;
        int u = graph->Edges[edge_idx].u;
        int v = graph->Edges[edge_idx].v;
        std::string edgeStr = std::to_string(u) + " - " + std::to_string(v);
        std::string weightStr = std::to_string(graph->Edges[edge_idx].w);
        
        algorithmBox.AddInfoLine("Current Edge", edgeStr);
        algorithmBox.AddInfoLine("Edge Weight", weightStr);

        // Show sets information
        int set_u = findSet(u);
        int set_v = findSet(v);
        algorithmBox.AddInfoLine("Set of u", std::to_string(set_u));
        algorithmBox.AddInfoLine("Set of v", std::to_string(set_v));
        
        if (set_u == set_v) {
            algorithmBox.AddInfoLine("Decision", "Skip (Same Set)");
        } else {
            algorithmBox.AddInfoLine("Decision", "Include in MST");
        }
    }
    
    // Show if algorithm is complete
    if (isKruskalFinished()) {
        algorithmBox.AddInfoLine("Status", "Algorithm Complete");
    }
}

bool GraphVisualizer::isKruskalFinished() const {
    return kruskal_data.current_edge_index >= (int)kruskal_data.edges.size();
}
#include "graph.h"

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
        kruskal_data.elapsed_time += GetFrameTime();
        if (kruskal_data.elapsed_time >= kruskal_data.step_interval) {
            if (kruskal_data.first_step) {
                kruskal_data.first_step = false;
            } else {
                GetKruskalStep();
            }
            kruskal_data.elapsed_time -= kruskal_data.step_interval;
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
}

